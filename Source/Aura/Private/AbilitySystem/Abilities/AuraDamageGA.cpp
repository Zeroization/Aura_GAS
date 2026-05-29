// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGA.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGA::CauseDamage(AActor* TargetActor)
{
    FGameplayEffectSpecHandle DamageGESpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
    for (const auto& [DamageElemTypeTag, ScaledDamageValue] : DamageElemToValue)
    {
        const float DamageValue = ScaledDamageValue.GetValueAtLevel(GetAbilityLevel());
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageGESpecHandle, DamageElemTypeTag, DamageValue);
    }
    GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageGESpecHandle.Data,
                                                                              UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
                                                                                  TargetActor));
}
