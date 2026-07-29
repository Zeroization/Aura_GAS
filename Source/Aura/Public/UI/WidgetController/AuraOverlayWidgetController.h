// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Data/TableRows/DisplayWidgetRow.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FDisplayWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityInfoSetSignature, const FAuraAbilityData&, AbilityData);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnAttributeChangedSignature OnMaxManaChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FMessageWidgetRowSignature OnDisplayMessageRowReceived;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")
    FOnAbilityInfoSetSignature OnAbilityInfoSet;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Stats")
    FOnPlayerStatChangedSignature OnXpPercentChanged;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Stats")
    FOnPlayerStatChangedSignature OnLevelNumberChanged;

    void BroadcastAbilityData();
    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UAuraAbilityInfo> AbilityInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UDataTable> MessageWidgetDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
    TObjectPtr<UDataTable> InputTagDisplayDataTable;

    void OnInitStartupAbilities();
    void OnXpChanged(int32 NewXp);
};
