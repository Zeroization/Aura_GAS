// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"


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
