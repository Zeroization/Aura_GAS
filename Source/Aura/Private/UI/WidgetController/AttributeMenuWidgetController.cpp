// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfo.h"
#include "Game/AuraGameplayTags.h"


void UAuraAttributeMenuWidgetController::BindDelegateCallbackFunctions()
{
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
	checkf(IsValid(AttributeInfo), TEXT("[%hs] AttributeInfo is empty, plz fill it in BP_AttributeMenuWidgetController !!"), __FUNCTION__);

	// 设置AttributeInfo中各属性的初值(只有Value无初值, 要向AttributeSet获取),
	// 并将其通过委托广播给UI
	FAuraAttributeData StrData = AttributeInfo->FindAttributeDataByTag(AuraGameplayTags::Attribute::Primary::Strength);
	StrData.AttributeValue = AuraAttributeSet->GetStrength();
	AttributeDataDelegate.Broadcast(StrData);
}
