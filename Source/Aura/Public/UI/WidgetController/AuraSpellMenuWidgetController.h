// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

class UAuraUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillBtnSelectedSignature, UAuraUserWidget*, SpellGlobeButton);

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
    
    UPROPERTY(BlueprintAssignable)
    FOnSkillBtnSelectedSignature OnSkillBtnSelected;
    
    UFUNCTION(BlueprintCallable)
    void Controller_SelectSpellButton(UAuraUserWidget* SpellButton);
    
    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;
};
