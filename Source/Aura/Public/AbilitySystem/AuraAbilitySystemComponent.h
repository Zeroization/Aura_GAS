// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityStatusChanged, const FGameplayTag& /* AbilityTag */,
                                       const FGameplayTag& /* SkillStatusTag */, int32 /* AbilityLevel */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetGEAssetTagDelegate, const FGameplayTagContainer& /* AssetTags */);
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

#pragma region GA
    FOnAbilitiesGivenDelegate OnAbilitiesGiven;
    FOnAbilityStatusChanged OnAbilityStatusChanged;
    bool bStartupAbilitiesGiven = false;

    UFUNCTION(Server, Reliable)
    void Server_SpendSkillPoint(const FGameplayTag& AbilityTag);

    /// 根据AbilityTag返回对应GA的描述, 找不到返回默认描述以及false 
    bool GetAbilityDescription(const FGameplayTag& AbilityTag, FString& OutDesc, FString& OutNextLevelDesc);

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
    /// 升级后检查并更新GA解锁信息
    void UpdateAbilityStatus(int32 NewLevel);

    FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag, bool bDebugLog = false);
    static FGameplayTag GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, bool bDebugLog = true);
    static FGameplayTag GetInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, bool bDebugLog = true);
    static FGameplayTag GetSkillStatusTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, bool bDebugLog = true);
#pragma endregion

#pragma region Attribute
    UFUNCTION(Server, Reliable)
    void Server_UpgradePrimaryAttribute(const FGameplayTag& PrimaryAttributeTag, int32 CostAttributePoint);

    void UpgradePrimaryAttribute(const FGameplayTag& PrimaryAttributeTag, int32 CostAttributePoint = 1);
#pragma endregion

protected:
    virtual void OnRep_ActivateAbilities() override;

    UFUNCTION(Client, Reliable)
    void OnGEApplied(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& InGESpec,
                     FActiveGameplayEffectHandle ActiveGEHandle);

    UFUNCTION(Server, Reliable)
    void Server_UpdateAbilityStatus();

    UFUNCTION(Client, Reliable)
    void Client_UpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag, int32 AbilityLevel);

private:
    static FGameplayTag GetTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& CategoryRootTag, bool bDebugLog);
};
