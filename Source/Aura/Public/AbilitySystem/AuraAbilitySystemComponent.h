// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetGEAssetTagDelegate, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FOnAbilitiesGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbilityExecuteDelegate, const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    FOnGetGEAssetTagDelegate OnGetGEAssetTag;
    FOnAbilitiesGivenDelegate OnAbilitiesGiven;

    bool bStartupAbilitiesGiven = false;

    /// 在 ASC->InitAbilityActorInfo() 被调用后执行
    void OnAbilityActorInfoSet();

    /// 赋予Actor默认GA
    void GrantActorStartupGAs(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilityClasses);

    /// 赋予Actor默认被动GA
    void GrantActorPassiveStartupGAs(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupPassiveAbilityClasses);

    /// 根据输入Tag触发的GA回调
    void AbilityInputTagOnHeld(const FGameplayTag& InputTag);
    void AbilityInputTagOnReleased(const FGameplayTag& InputTag);

    /// 让每个GA执行一次FForEachAbilityExecuteDelegate回调
    void ForEachAbility(const FForEachAbilityExecuteDelegate& DelegateToExecute);

    static FGameplayTag GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec);

protected:
    virtual void OnRep_ActivateAbilities() override;

    UFUNCTION(Client, Reliable)
    void OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
                     FActiveGameplayEffectHandle ActiveGEHandle);
};
