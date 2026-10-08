// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuWidgetController::Controller_SelectSpellButton(UAuraUserWidget* SpellButton, const FGameplayTag& AbilityTag)
{
    FSelectedBtnData Data;
    if (AuraAbilitySystemComponent)
    {
        AuraAbilitySystemComponent->GetAbilityDescription(AbilityTag, Data.SkillDesc, Data.NextLevelSkillDesc);
    }
    OnUpdateSkillDesc.Broadcast(Data, AbilityTag);
    OnSkillBtnSelected.Broadcast(SpellButton);
}

void UAuraSpellMenuWidgetController::Controller_DeselectSpellButton(UAuraUserWidget* SpellButton)
{
    OnSkillBtnDisSelected.Broadcast(SpellButton);
}

void UAuraSpellMenuWidgetController::Controller_OnSpendSkillPointPressed(const FGameplayTag& AbilityTag)
{
    FSelectedBtnData Data;
    if (AuraAbilitySystemComponent)
    {
        AuraAbilitySystemComponent->Server_SpendSkillPoint(AbilityTag);
        AuraAbilitySystemComponent->GetAbilityDescription(AbilityTag, Data.SkillDesc, Data.NextLevelSkillDesc);
    }
    OnUpdateSkillDesc.Broadcast(Data, AbilityTag);
}

void UAuraSpellMenuWidgetController::BroadcastInitialValues()
{
    if (AuraAbilitySystemComponent && AuraPlayerState)
    {
        AuraAbilitySystemComponent->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
    }

    BroadcastInitialAbilities();
    OnSkillPointChange.Broadcast(AuraPlayerState->GetSkillPoint());
}

void UAuraSpellMenuWidgetController::BindDelegateCallbackFunctions()
{
#pragma region AuraASC
    AuraAbilitySystemComponent->OnAbilityStatusChanged.AddLambda(
        [this](const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag, int32 AbilityLevel)
        {
            if (AbilityInfo)
            {
                FAuraAbilityData* Data = AbilityInfo->ModifyAbilityDataByTag(AbilityTag);
                Data->SkillStatusTag = SkillStatusTag;
                // 看情况添加: Data->Level = AbilityLevel;
                OnAbilityInfoSet.Broadcast(*Data);
            }
        });
#pragma endregion

#pragma region AuraPS
    AuraPlayerState->OnPlayerSkillPointChange.AddLambda([this](int32 SkillPoint)
    {
        OnSkillPointChange.Broadcast(SkillPoint);
    });
    AuraPlayerState->OnPlayerLevelChange.AddLambda([this](int32 Level)
    {
        OnLevelNumberChanged.Broadcast(Level);
    });
#pragma endregion
}
