// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/DataAssets/AuraAttributeInfo.h"


void UAuraAttributeMenuWidgetController::BindDelegateCallbackFunctions()
{
    checkf(IsValid(AttributeInfo), TEXT("[%hs]: AttributeInfo is empty, plz fill it in BP_AttributeMenuWidgetController !!"),
           __FUNCTION__);

    for (FAuraAttributeData& AttributeData : AttributeInfo->AttributeDataList)
    {
        AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeData.Attribute).AddLambda(
            [this, &AttributeData](const FOnAttributeChangeData& ChangedData)
            {
                BroadcastAttributeData(AttributeData);
            }
        );
    }
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
    checkf(IsValid(AttributeInfo), TEXT("[%hs]: AttributeInfo is empty, plz fill it in BP_AttributeMenuWidgetController !!"),
           __FUNCTION__);

    for (FAuraAttributeData& AttributeData : AttributeInfo->AttributeDataList)
    {
        BroadcastAttributeData(AttributeData);
    }
}

void UAuraAttributeMenuWidgetController::BroadcastAttributeData(FAuraAttributeData& AttributeData) const
{
    AttributeData.AttributeValue = AttributeData.Attribute.GetNumericValue(AuraAttributeSet);
    AttributeDataDelegate.Broadcast(AttributeData);
}
