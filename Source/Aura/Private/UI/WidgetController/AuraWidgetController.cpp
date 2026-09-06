// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"


void UAuraWidgetController::SetWidgetControllerParams(const FAuraWidgetControllerParams& InParams)
{
    AuraPlayerController = InParams.PlayerController;
    AuraPlayerState = InParams.PlayerState;
    AuraAbilitySystemComponent = InParams.AbilitySystemComponent;
    AuraAttributeSet = InParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindDelegateCallbackFunctions()
{
}

void UAuraWidgetController::BroadcastInitialAbilities()
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
        if (Data.SkillTypeTag.MatchesTagExact(AuraGameplayTags::GA::SkillType::None))
        {
            return;
        }
        Data.InputTag = AuraAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);

        // 2. 向UI组件广播相关信息
        OnAbilityInfoSet.Broadcast(Data);
    });
    AuraAbilitySystemComponent->ForEachAbility(DelegateToExecute);
}
