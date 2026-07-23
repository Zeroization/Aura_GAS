// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameplayTags.h"

UGameplayEffect* UAuraGameplayAbility::GetCostGameplayEffect() const
{
    if (CostGameplayEffectClass)
    {
        UGameplayEffect* CostGE = NewObject<UGameplayEffect>(
            GetTransientPackage(),
            CostGameplayEffectClass
        );
        CostGE->DurationPolicy = EGameplayEffectDurationType::Instant;

        // 动态创建消耗Modifier
        int32 Idx = CostGE->Modifiers.Num();
        CostGE->Modifiers.SetNum(Idx + Costs.Num());
        for (const FAbilityCost& Cost : Costs)
        {
            FGameplayModifierInfo Modifier;
            Modifier.Attribute = Cost.CostAttribute;
            Modifier.ModifierOp = EGameplayModOp::Additive;
            Modifier.ModifierMagnitude =
                FGameplayEffectModifierMagnitude(FScalableFloat(Cost.CostValue));

            CostGE->Modifiers[Idx] = Modifier;
            ++Idx;
        }
        return CostGE;
    }
    return nullptr;
}

void UAuraGameplayAbility::ApplyCost(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo
) const
{
    if (UGameplayEffect* CostGE = GetCostGameplayEffect())
    {
        UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
        FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
        FGameplayEffectSpec Spec(CostGE, Context, GetAbilityLevel());

        ASC->ApplyGameplayEffectSpecToSelf(Spec);
    }
}

const FGameplayTagContainer* UAuraGameplayAbility::GetCooldownTags() const
{
    CachedCooldownTags.Reset();

    if (const FGameplayTagContainer* Parent = Super::GetCooldownTags())
    {
        CachedCooldownTags.AppendTags(*Parent);
    }
    CachedCooldownTags.AppendTags(CooldownTag);

    return &CachedCooldownTags;
}

void UAuraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo) const
{
    if (UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
    {
        FGameplayEffectSpecHandle CooldownGESpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
        CooldownGESpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTag);
        CooldownGESpecHandle.Data->SetSetByCallerMagnitude(AuraGameplayTags::Cooldown::Root,
                                                           CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
        
        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CooldownGESpecHandle);
    }
}
