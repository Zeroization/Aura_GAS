// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGEApplied);
}

void UAuraAbilitySystemComponent::OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
                                              FActiveGameplayEffectHandle ActiveGEHandle)
{
	FString DebugMsg = FString::Printf(TEXT("GE %s has been applied."), *InGESpec.Def->GetName());
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, DebugMsg);
}
