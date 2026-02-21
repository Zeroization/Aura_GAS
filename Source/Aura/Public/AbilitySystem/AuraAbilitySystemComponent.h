// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetGEAssetTagDelegate, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FOnGetGEAssetTagDelegate OnGetGEAssetTag;

	/// 在 ASC->InitAbilityActorInfo() 被调用后执行
	void OnAbilityActorInfoSet();

	/// 赋予Actor GA
	void GrantActorGA(const TArray<TSubclassOf<UAuraGameplayAbility>>& AbilityClasses);

	/// 根据输入Tag触发的GA回调
	void AbilityInputTagOnHeld(const FGameplayTag& InputTag);
	void AbilityInputTagOnReleased(const FGameplayTag& InputTag);

protected:
	void OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
	                 FActiveGameplayEffectHandle ActiveGEHandle);
};
