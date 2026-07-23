// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FAbilityCost
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    FGameplayAttribute CostAttribute;

    UPROPERTY(EditDefaultsOnly)
    FScalableFloat CostValue;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    /// 触发GA的默认输入Tag
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Input")
    FGameplayTag StartupInputTag;

#pragma region Cost
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Cost")
    TArray<FAbilityCost> Costs;

    virtual UGameplayEffect* GetCostGameplayEffect() const override;
    virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                           const FGameplayAbilityActivationInfo ActivationInfo) const override;
#pragma endregion

#pragma region Cooldown
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Cooldown")
    FScalableFloat CooldownDuration;

    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Cooldown")
    FGameplayTagContainer CooldownTag;

    // 临时容器, 用于在GetCooldownTags()中返回最终结果
    mutable FGameplayTagContainer CachedCooldownTags;

    virtual const FGameplayTagContainer* GetCooldownTags() const override;
    virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo) const override;
#pragma endregion
};
