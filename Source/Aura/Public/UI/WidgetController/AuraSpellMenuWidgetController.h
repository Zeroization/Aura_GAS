// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
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

    UPROPERTY(BlueprintReadWrite)
    FGameplayTag SkillStatusTag;

    FSelectedBtnData() = default;

    FSelectedBtnData(UAuraAbilitySystemComponent* AuraASC, const FGameplayTag& SelectedAbilityTag)
    {
        if (AuraASC)
        {
            AuraASC->GetAbilityDescription(SelectedAbilityTag, this->SkillDesc, this->NextLevelSkillDesc);
            this->SkillStatusTag = AuraASC->GetSkillStatusTagByAbilityTag(SelectedAbilityTag);
        }
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopWaitingForEquipSkillSignature);

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

    UPROPERTY(BlueprintAssignable, Category = "GAS|UI")
    FOnStopWaitingForEquipSkillSignature OnStopWaitingForEquipSkill;

    UFUNCTION(BlueprintCallable)
    void Controller_SelectSpellButton(UAuraUserWidget* SpellButton, const FGameplayTag& AbilityTag);

    UFUNCTION(BlueprintCallable)
    void Controller_DeselectSpellButton(UAuraUserWidget* SpellButton);

    UFUNCTION(BlueprintCallable)
    void Controller_OnSpendSkillPointPressed(const FGameplayTag& AbilityTag);

    void OnSkillEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag,
                         const FGameplayTag& NewInputTag, const FGameplayTag& PrevInputTag);

    virtual void BroadcastInitialValues() override;
    virtual void BindDelegateCallbackFunctions() override;
};
