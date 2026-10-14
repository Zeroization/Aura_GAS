// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGA.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"

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

FAuraDamageEffectParams UAuraDamageGA::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor)
{
    FAuraDamageEffectParams Params;
    Params.WorldContextObject = GetAvatarActorFromActorInfo();
    Params.DamageEffectClass = DamageEffectClass;
    Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
    Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    for (auto& [DamageElemTypeTag, DamageScalableFloat] : DamageElemToValue)
    {
        Params.DamageElemTypeToBaseDamage.Add(DamageElemTypeTag, DamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));
    }
    for (auto& [DamageElemTypeTag, DebuffParams] : DamageElemToDebuffParam)
    {
        Params.DamageElemTypeToDebuffParams.Add(DamageElemTypeTag, DebuffParams);
    }
    Params.AbilityLevel = GetAbilityLevel();

    return Params;
}

bool UAuraDamageGA::GetDescFormatArgs(FStringFormatNamedArguments& Args, int32 AbilityLevel)
{
    if (Super::GetDescFormatArgs(Args, AbilityLevel))
    {
        for (auto& [DamageTypeTag, DamageScaledValue] : DamageElemToValue)
        {
            int32 DamageValue = DamageScaledValue.GetValueAtLevel(AbilityLevel);
            if (DamageTypeTag.MatchesTagExact(AuraGameplayTags::Damage::ElemType::Physical))
            {
                Args.Add(TEXT("_PhysicalDamage"), DamageValue);
            }
            else if (DamageTypeTag.MatchesTagExact(AuraGameplayTags::Damage::ElemType::Fire))
            {
                Args.Add(TEXT("_FireDamage"), DamageValue);
            }
            else if (DamageTypeTag.MatchesTagExact(AuraGameplayTags::Damage::ElemType::Lightning))
            {
                Args.Add(TEXT("_LightningDamage"), DamageValue);
            }
            else if (DamageTypeTag.MatchesTagExact(AuraGameplayTags::Damage::ElemType::Arcane))
            {
                Args.Add(TEXT("_ArcaneDamage"), DamageValue);
            }
            else
            {
                UE_LOG(LogAuraGame, Warning, TEXT("[%hs]: Unknown damage tag %s with damage value %d"), __FUNCTION__,
                       *DamageTypeTag.ToString(), DamageValue);
                return false;
            }
        }
    }
    return true;
}
