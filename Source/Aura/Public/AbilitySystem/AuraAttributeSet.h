// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define AURA_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() = default;

	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;

	// Source: 引发GE的Actor
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> SourceController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	// Target: 被GE施加效果的Actor
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor = nullptr;

	UPROPERTY()
	TObjectPtr<AController> TargetController = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// Begin Category: Vital Attributes 必备属性 >>>>>>>>>>>>>
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes|Vital Attributes")
	FGameplayAttributeData Health;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Attributes|Vital Attributes")
	FGameplayAttributeData Mana;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	// End Category: Vital Attributes   >>>>>>>>>>>>>

	// Begin Category: Primary Attributes 首要属性: 值独立设置, 不依赖其他属性 >>>>>>>>>>>>>>>>>
	/// STR: 增加物理攻击伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Attributes|Primary Attributes")
	FGameplayAttributeData Strength;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	/// INT: 增加魔法攻击伤害和持续回蓝 ManaRegeneration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Attributes|Primary Attributes")
	FGameplayAttributeData Intelligence;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	/// RES: 增加护甲值 Armor 和护甲穿透 ArmorPenetration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Attributes|Primary Attributes")
	FGameplayAttributeData Resilience;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	/// VIG: 增加生命值 Health 和持续回血 HealthRegeneration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Attributes|Primary Attributes")
	FGameplayAttributeData Vigor;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
	// End Category: Primary Attributes >>>>>>>>>>>>>>>>>>>

	// Begin Category: Secondary Attributes 次级属性: 值有依赖一级属性或部分次级属性 >>>>>>>>>>>>>>>>>>>>>>>>>>
	/// Mana Regeneration: 依赖属性INT, 每秒持续回复特定蓝值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	/// Max Mana: 依赖属性INT, Mana的最大值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData MaxMana;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	/// Armor: 依赖属性RES, 减少收到的伤害, 增加格挡率 BlockChance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData Armor;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	/// Block Chance: 依赖属性Armor, 概率使受到伤害减半
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData BlockChance;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	/// Critical Hit Resistance: 依赖属性Armor, 减少来自敌人攻击的暴击率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	/// Armor Penetration: 依赖属性RES, 忽略敌人部分护甲值, 增加暴击率 CriticalHitChance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	/// Critical Hit Chance: 依赖属性ArmorPenetration, 概率造成双倍伤害+暴击补偿伤害CriticalHitBonusDamage
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	/// Critical Hit Bonus Damage: 依赖属性ArmorPenetration, 暴击补偿伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitBonusDamage, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData CriticalHitBonusDamage;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitBonusDamage);

	/// Health Regeneration: 依赖属性VIG, 每秒持续回复特定生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	/// Max Health: 依赖属性VIG, Health的最大值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes|Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	AURA_ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	// End Category: Secondary Attributes

	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// Begin: OnRep_ Functions >>>>>>>>>>>>>>>>>>>>>>
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_CriticalHitBonusDamage(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldData) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldData) const;
	// End: OnRep_ Functions   >>>>>>>>>>>>>>>>>>>>>>

private:
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
