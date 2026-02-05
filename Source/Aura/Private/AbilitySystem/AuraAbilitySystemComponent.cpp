// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGEApplied);
}

void UAuraAbilitySystemComponent::GrantActorGA(const TArray<TSubclassOf<UAuraGameplayAbility>>& AbilityClasses)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, 1);

		// GiveAbility(GASpec);
		GiveAbilityAndActivateOnce(GASpec);
	}
}

void UAuraAbilitySystemComponent::OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
                                              FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	InGESpec.GetAllAssetTags(TagContainer);

	// 广播获取的Tag到UIWidgetController
	OnGetGEAssetTag.Broadcast(TagContainer);
}
