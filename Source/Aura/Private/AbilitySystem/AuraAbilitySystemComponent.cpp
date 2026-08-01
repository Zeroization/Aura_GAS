// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"
#include "Interaction/Interface/PlayerInterface.h"

void UAuraAbilitySystemComponent::OnAbilityActorInfoSet()
{
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGEApplied);
}

void UAuraAbilitySystemComponent::GrantActorStartupGAs(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilityClasses)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilityClasses)
    {
        FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, 1);
        if (const UAuraGameplayAbility* AuraGA = Cast<UAuraGameplayAbility>(GASpec.Ability))
        {
            GASpec.DynamicAbilityTags.AddTag(AuraGA->StartupInputTag);
            GiveAbility(GASpec);
        }
    }

    bStartupAbilitiesGiven = true;
    OnAbilitiesGiven.Broadcast();
}

void UAuraAbilitySystemComponent::GrantActorPassiveStartupGAs(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupPassiveAbilityClasses)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilityClasses)
    {
        FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, 1);
        GiveAbilityAndActivateOnce(GASpec);
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

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbilityExecuteDelegate& DelegateToExecute)
{
    // 该锁可以防止遍历GA过程中部分GA的状态发生变化
    FScopedAbilityListLock Lock(*this);
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (!DelegateToExecute.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Failed to execute delegate in GA %s"), __FUNCTION__, *AbilitySpec.Ability.GetName())
        }
    }
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if (IsValid(AbilitySpec.Ability))
    {
        for (FGameplayTag Tag : AbilitySpec.Ability->AbilityTags)
        {
            if (Tag.MatchesTag(AuraGameplayTags::GA::Root))
            {
                return Tag;
            }
        }
    }

    UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Can't get ability tag in GA %s"), __FUNCTION__, *AbilitySpec.Ability.GetName());
    return {};
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagByAbilitySpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
    {
        if (Tag.MatchesTag(AuraGameplayTags::Input::Root))
        {
            return Tag;
        }
    }

    UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Can't get input tag in GA %s"), __FUNCTION__, *AbilitySpec.Ability.GetName());
    return {};
}

void UAuraAbilitySystemComponent::Server_UpgradePrimaryAttribute_Implementation(const FGameplayTag& PrimaryAttributeTag,
                                                                                int32 CostAttributePoint)
{
    // 1. 升级对应属性
    FGameplayEventData Payload;
    Payload.EventTag = PrimaryAttributeTag;
    Payload.EventMagnitude = CostAttributePoint;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), PrimaryAttributeTag, Payload);

    // 2. 减少属性点
    IPlayerInterface::Execute_PlayerAddAttributePoint(GetAvatarActor(), -CostAttributePoint);
}

void UAuraAbilitySystemComponent::UpgradePrimaryAttribute(const FGameplayTag& PrimaryAttributeTag, int32 CostAttributePoint)
{
    if (GetAvatarActor()->Implements<UPlayerInterface>() &&
        IPlayerInterface::Execute_PlayerGetAttributePointValue(GetAvatarActor()) > 0)
    {
        Server_UpgradePrimaryAttribute(PrimaryAttributeTag, CostAttributePoint);
    }
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    // 通过重写此函数可以让客户端同步GA的数据多播
    if (!bStartupAbilitiesGiven)
    {
        bStartupAbilitiesGiven = true;
        OnAbilitiesGiven.Broadcast();
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
