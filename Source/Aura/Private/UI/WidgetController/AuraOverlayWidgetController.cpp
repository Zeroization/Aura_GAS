// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"
#include "Tools/AuraDataTableFunctionLibrary.h"


void UAuraOverlayWidgetController::BroadcastAbilityData()
{
    if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
    {
        // 如果在绑定委托前已经获取到初始GA, 直接执行对应逻辑
        OnInitStartupAbilities();
    }
}

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UAuraOverlayWidgetController::BindDelegateCallbackFunctions()
{
    // UAuraOverlayWidgetController::FOnAttributeChangedSignature
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnHealthChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnMaxHealthChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnManaChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnMaxManaChanged.Broadcast(Data.NewValue);
                              });

    // UAuraAbilitySystemComponent::FOnGetGEAssetTagDelegate
    AuraAbilitySystemComponent->OnGetGEAssetTag.AddLambda([this](const FGameplayTagContainer& AssetTags)
    {
        for (const auto& Tag : AssetTags)
        {
#ifdef WITH_EDITOR
            const FString DebugMsg = FString::Printf(TEXT("[Overlay UI Controller] GE Tag: %s"), *Tag.ToString());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DebugMsg);
#endif

            // 只有Tag是 UI.DisplayMessage.* 的才会查表, 并将其广播至显示对应消息的UI
            if (FGameplayTag DisplayMessageTag = FGameplayTag::RequestGameplayTag(
                    AuraGameplayTags::UI::DisplayMessage::Root.GetTag().GetTagName());
                Tag.MatchesTag(DisplayMessageTag))
            {
                const FDisplayWidgetRow* Row = FAuraDataTableFunctionLibrary::GetDataTableRowByGameplayTag<FDisplayWidgetRow>(
                    MessageWidgetDataTable, Tag);
                OnDisplayMessageRowReceived.Broadcast(*Row);
            }
        }
    });

    // UAuraAbilitySystemComponent::FOnAbilitiesGivenDelegate
    AuraAbilitySystemComponent->OnAbilitiesGiven.AddUObject(this, &UAuraOverlayWidgetController::OnInitStartupAbilities);
}

void UAuraOverlayWidgetController::OnInitStartupAbilities()
{
    if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)
    {
        return;
    }

    // 给每个初始GA绑定UI相关回调, 让ASC执行回调
    FForEachAbilityExecuteDelegate DelegateToExecute;
    DelegateToExecute.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
    {
        // 1. 在AbilityInfo中获取并补充Ability相关信息
        FAuraAbilityData Data = AbilityInfo->FindAbilityDataByTag(AuraAbilitySystemComponent->GetAbilityTagByAbilitySpec(AbilitySpec));
        Data.InputTag = AuraAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
        
        // 2. 向UI组件广播相关信息
        OnAbilityInfoSet.Broadcast(Data);
    });
    AuraAbilitySystemComponent->ForEachAbility(DelegateToExecute);
}
