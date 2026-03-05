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
		if (const UAuraGameplayAbility* AuraGA = Cast<UAuraGameplayAbility>(GASpec.Ability))
		{
			GASpec.DynamicAbilityTags.AddTag(AuraGA->StartupInputTag);
			GiveAbility(GASpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagOnHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (TArray<FGameplayAbilitySpec> ActivatableGASpecs = GetActivatableAbilities();
		 FGameplayAbilitySpec& GASpec : ActivatableGASpecs)
	{
		// 寻找该按键输入能触发的GA
		if (GASpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 标识该GA的输入触发为: 已按下
			AbilitySpecInputPressed(GASpec);
			// 如果该GA仍未激活, 激活它
			if (!GASpec.IsActive())
			{
				TryActivateAbility(GASpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagOnReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (TArray<FGameplayAbilitySpec> ActivatableGASpecs = GetActivatableAbilities();
		 FGameplayAbilitySpec& GASpec : ActivatableGASpecs)
	{
		if (GASpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(GASpec);
		}
	}
}

void UAuraAbilitySystemComponent::OnGEApplied_Implementation(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
	FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	InGESpec.GetAllAssetTags(TagContainer);

	// 广播获取的Tag到UIWidgetController
	OnGetGEAssetTag.Broadcast(TagContainer);
}
