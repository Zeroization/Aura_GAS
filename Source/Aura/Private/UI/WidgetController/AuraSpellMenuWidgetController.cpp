// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuWidgetController::Controller_SelectSpellButton(UAuraUserWidget* SpellButton)
{
    OnSkillBtnSelected.Broadcast(SpellButton);
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
    AuraAbilitySystemComponent->OnAbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag)
    {
        if (AbilityInfo)
        {
            FAuraAbilityData* Data = AbilityInfo->ModifyAbilityDataByTag(AbilityTag);
            Data->SkillStatusTag = SkillStatusTag;
            OnAbilityInfoSet.Broadcast(*Data);
        }
    });
#pragma endregion

#pragma region AuraPS
    AuraPlayerState->OnPlayerSkillPointChange.AddLambda([this](int32 SkillPoint)
    {
        OnSkillPointChange.Broadcast(SkillPoint);
    });
#pragma endregion
}
