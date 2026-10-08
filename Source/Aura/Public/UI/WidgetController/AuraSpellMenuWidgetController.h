// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FSelectedBtnData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString SkillDesc;

    UPROPERTY(BlueprintReadWrite)
    FString NextLevelSkillDesc;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillBtnSelectedSignature, UAuraUserWidget*, SpellGlobeButton);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateSkillDescSignature, FSelectedBtnData, Data, const FGameplayTag&, AbilityTag);

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

    UPROPERTY(BlueprintAssignable, Category = "GAS|UI")
    FOnSkillBtnSelectedSignature OnSkillBtnDisSelected;

    UPROPERTY(BlueprintAssignable, Category = "GAS|UI")
    FOnUpdateSkillDescSignature OnUpdateSkillDesc;

    UFUNCTION(BlueprintCallable)
    void Controller_SelectSpellButton(UAuraUserWidget* SpellButton, const FGameplayTag& AbilityTag);

    UFUNCTION(BlueprintCallable)
    void Controller_DeselectSpellButton(UAuraUserWidget* SpellButton);

    UFUNCTION(BlueprintCallable)
    void Controller_OnSpendSkillPointPressed(const FGameplayTag& AbilityTag);

    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;
};
