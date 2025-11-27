// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

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

	/**
	 *  在 `ASC->InitAbilityActorInfo()` 被调用后执行
	 */
	void OnAbilityActorInfoSet();

protected:
	void OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
	                 FActiveGameplayEffectHandle ActiveGEHandle);
};
