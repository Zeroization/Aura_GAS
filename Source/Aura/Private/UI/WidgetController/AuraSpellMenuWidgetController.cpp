// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"

void UAuraSpellMenuWidgetController::BroadcastInitialValues()
{
    BroadcastInitialAbilities();
}

void UAuraSpellMenuWidgetController::BindDelegateCallbackFunctions()
{
#pragma region AuraASC
    AuraAbilitySystemComponent->OnAbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag)
    {
        if (AbilityInfo)
        {
            FAuraAbilityData Data = AbilityInfo->FindAbilityDataByTag(AbilityTag);
            Data.SkillStatusTag = SkillStatusTag;
            OnAbilityInfoSet.Broadcast(Data);
        }
    });
#pragma endregion
}
