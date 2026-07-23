// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"


UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChangeProxy(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{
    UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
    WaitCooldownChange->ASC = InASC;
    WaitCooldownChange->CooldownTag = InCooldownTag;

    if (!IsValid(InASC) || !InCooldownTag.IsValid())
    {
        WaitCooldownChange->EndTask();
        return nullptr;
    }

    WaitCooldownChange->ASC->RegisterGameplayTagEvent(WaitCooldownChange->CooldownTag, EGameplayTagEventType::NewOrRemoved)
                      .AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnCooldownTagChanged);
    WaitCooldownChange->ASC->OnActiveGameplayEffectAddedDelegateToSelf
                      .AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveGEAdded);

    return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
    if (IsValid(ASC))
    {
        ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
    }

    SetReadyToDestroy();
    MarkAsGarbage();
}

void UWaitCooldownChange::OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
    if (NewCount == 0)
    {
        OnCooldownEnd.Broadcast(0.f);
    }
}

void UWaitCooldownChange::OnActiveGEAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec,
                                          FActiveGameplayEffectHandle ActiveGEHandle)
{
    // 在触发冷却GE后, 该回调会被本地和网络预测激活两次, 因此需要筛除一种情况
    if (bool bIsReplicatedGE = !GESpec.GetContext().GetAbilityInstance_NotReplicated();
        bIsReplicatedGE)
    {
        return;
    }
    
    FGameplayTagContainer AssetTags, GrantedTags;
    GESpec.GetAllAssetTags(AssetTags);
    GESpec.GetAllGrantedTags(GrantedTags);

    if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
    {
        FGameplayEffectQuery GEQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
        float TimeRemaining = FMath::Max(ASC->GetActiveEffectsTimeRemaining(GEQuery));
        OnCooldownStart.Broadcast(TimeRemaining);
    }
}
