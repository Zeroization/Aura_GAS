// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/Interface/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UMotionWarpingComponent;
class UAuraGameplayAbility;
class UGameplayEffect;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE UAuraAttributeSet* GetAttributeSet() const { return AuraAttributeSet; }

protected:
	/// Begin Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	/// TObjectPtr<>是UE5新引入的指针类型, 用于替代传统UObject*, 更安全高效
	/// 有两个关键特性:
	///		1. 访问追踪 Access Tracking:	记录和检测TObjectPtr<>的访问情况,
	///									利于编辑器追踪UObject*, 对其进行
	///									额外检查.
	///		2. 延迟加载 Lazy Loading:	当对象来自外部包时，它可以保存一个轻量级
	///								的句柄，在访问时才真正加载对象。
	UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
	FName WeaponTipSocketName;

	/// 所有客户端: 该角色死亡的逻辑(例如播放动画等)
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastOnCharacterDeath();
	/// 服务器: 该角色死亡的逻辑
	virtual void Die() override;
	virtual FVector GetWeaponSocketLocation_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	/// End Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	/// Begin: Ability System <<<<<<<<<<<<<<<
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
	TSubclassOf<UGameplayEffect> InitPrimaryAttrGEClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
	TSubclassOf<UGameplayEffect> InitSecondaryAttrGEClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
	TSubclassOf<UGameplayEffect> InitVitalAttrGEClass;

	virtual void InitAbilitySystem();
	virtual void InitDefaultAttributes() const;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GEClass, float Level = 1.f) const;
	void GrantCharacterStartupAbilities();
	/// End: Ability System   <<<<<<<<<<<<<<<

	/// Begin: Animation <<<<<<<<<<<<<<<<<<<<
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AuraCharacter|Animation")
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UFUNCTION(BlueprintCallable)
	bool CheckMotionWarpTargetExists(const FName& WarpTargetName);
	/// End:   Animation <<<<<<<<<<<<<<<<<<<<

	/* Begin: SFX */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/// 在角色死亡时替换溶解材质
	void Dissolve();

	/// 在蓝图中实现角色慢慢溶解的效果
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMIs);
	/* End: SFX */

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "AuraCharacter|Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
