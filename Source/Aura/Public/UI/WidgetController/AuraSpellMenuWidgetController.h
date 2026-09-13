// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraSpellMenuWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnPlayerStatChangedSignature OnSkillPointChange;
    
    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;
};
