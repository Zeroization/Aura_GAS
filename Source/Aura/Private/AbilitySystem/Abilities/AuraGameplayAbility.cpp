// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"

FString UAuraGameplayAbility::GetDescription(int32 AbilityLevel, UObject* WorldContextObject)
{
    return FString::Printf(TEXT("默认技能名 - 饿啊, 等级: <Level>%d</>"), AbilityLevel);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 NextLevel, UObject* WorldContextObject)
{
    return FString::Printf(TEXT("下一级: <Level>%d</>, 大量伤害?!"), NextLevel);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 LevelRequirement)
{
    return FString::Printf(TEXT("该技能被锁定, 解锁等级为: <Level>%d</>"), LevelRequirement);
}

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
        CooldownGESpecHandle.Data->SetSetByCallerMagnitude(AuraGameplayTags::GA::Cooldown::Root,
                                                           CooldownDuration.GetValueAtLevel(GetAbilityLevel()));

        ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CooldownGESpecHandle);
    }
}

bool UAuraGameplayAbility::GetDescFormatArgs(FStringFormatNamedArguments& Args, int32 AbilityLevel)
{
    // Level
    Args.Add(TEXT("_AbilityLevel"), {AbilityLevel});
    // Cost
    FString CostFormattedString;
    for (int i = 0; i < Costs.Num(); ++i)
    {
        if (Costs[i].CostAttribute.AttributeName.Equals(AURA_ATTRIBUTE_NAME_MANA))
        {
            CostFormattedString.Append(FString::Printf(TEXT("<Cost_Mana>魔力 %.1f</>"), Costs[i].CostValue.GetValueAtLevel(AbilityLevel)));
        }
        else
        {
            CostFormattedString.Append(FString::Printf(TEXT("未定义属性%s(%.1f)"), *Costs[i].CostAttribute.AttributeName,
                                                       Costs[i].CostValue.GetValueAtLevel(AbilityLevel)));

            UE_LOG(LogAuraGame, Warning, TEXT("[%hs]: Unknown cost attribute [%s] with cost value %.1f"), __FUNCTION__,
                   *Costs[i].CostAttribute.AttributeName,
                   Costs[i].CostValue.GetValueAtLevel(AbilityLevel))
            return false;
        }

        if (i + 1 < Costs.Num())
        {
            CostFormattedString.Append(TEXT(", "));
        }
    }
    Args.Add(TEXT("_Cost"), CostFormattedString);
    // CD
    Args.Add(TEXT("_CD"), FString::Printf(TEXT("%.1f"), CooldownDuration.GetValueAtLevel(AbilityLevel)));

    return true;
}
