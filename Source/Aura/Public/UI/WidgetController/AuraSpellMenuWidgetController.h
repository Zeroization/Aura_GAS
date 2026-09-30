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
    UPROPERTY(BlueprintAssignable, Category = "GAS|Stats")
    FOnPlayerStatChangedSignature OnSkillPointChange;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Stats")
    FOnPlayerStatChangedSignature OnLevelNumberChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "GAS|UI")
    FOnSkillBtnSelectedSignature OnSkillBtnSelected;

    UFUNCTION(BlueprintCallable)
    void Controller_SelectSpellButton(UAuraUserWidget* SpellButton);

    UFUNCTION(BlueprintCallable)
    void Controller_OnSpendSkillPointPressed(const FGameplayTag& AbilityTag);

    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;
};
