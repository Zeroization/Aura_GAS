// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
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
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	/// End Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	/// Begin: Ability System <<<<<<<<<<<<<<<
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitPrimaryAttrGEClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitSecondaryAttrGEClass;

	virtual void InitAbilitySystem();

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GEClass, float Level = 1.f) const;
	void InitDefaultAttributes() const;
	/// End: Ability System   <<<<<<<<<<<<<<<

	virtual void BeginPlay() override;
};
