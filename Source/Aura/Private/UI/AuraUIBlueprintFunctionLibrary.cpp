// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AuraUIBlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "Tools/AuraDataTableFunctionLibrary.h"
#include "UI/Data/TableRows/InputTagDisplayRow.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

FText UAuraUIBlueprintFunctionLibrary::GetDisplayNameByInputTag(UDataTable* InputTagDisplayDataTable, const FGameplayTag& InputTag)
{
    checkf(IsValid(InputTagDisplayDataTable), TEXT("[%hs] Invalid data table"), __FUNCTION__);
    if (InputTag.MatchesTag(AuraGameplayTags::Input::Root))
    {
        const FInputTagDisplayRow* Row = FAuraDataTableFunctionLibrary::GetDataTableRowByGameplayTag<FInputTagDisplayRow>(
            InputTagDisplayDataTable, InputTag);
        return Row->DisplayName;
    }

    UE_LOG(LogAuraGame, Warning, TEXT("[%hs] Invalid input tag: %s"), __FUNCTION__, *InputTag.GetTagName().ToString());
    return FText::FromName(TEXT("未知输入"));
}

UAuraOverlayWidgetController* UAuraUIBlueprintFunctionLibrary::GetAuraOverlayWidgetController(const UObject* WorldContextObject)
{
    FAuraWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeAuraWidgetControllerParam(WorldContextObject, WCParams, AuraHUD))
    {
        return AuraHUD->GetAuraOverlayWidgetController(WCParams);
    }

    return nullptr;
}

UAuraAttributeMenuWidgetController* UAuraUIBlueprintFunctionLibrary::GetAuraAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    FAuraWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeAuraWidgetControllerParam(WorldContextObject, WCParams, AuraHUD))
    {
        return AuraHUD->GetAuraAttributeMenuWidgetController(WCParams);
    }

    return nullptr;
}

UAuraSpellMenuWidgetController* UAuraUIBlueprintFunctionLibrary::GetAuraSpellMenuWidgetController(const UObject* WorldContextObject)
{
    FAuraWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeAuraWidgetControllerParam(WorldContextObject, WCParams, AuraHUD))
    {
        return AuraHUD->GetAuraSpellMenuWidgetController(WCParams);
    }

    return nullptr;
}

bool UAuraUIBlueprintFunctionLibrary::MakeAuraWidgetControllerParam(const UObject* WorldContextObject,
                                                                    FAuraWidgetControllerParams& OutWCParam, AAuraHUD*& OutAuraHUD)
{
    // 从本地玩家获取UI Controller
    if (AAuraPlayerController* PlayerController =
        Cast<AAuraPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
        {
            AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
            UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
            UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());

            OutWCParam.AbilitySystemComponent = ASC;
            OutWCParam.AttributeSet = AttributeSet;
            OutWCParam.PlayerController = PlayerController;
            OutWCParam.PlayerState = PlayerState;
            OutAuraHUD = AuraHUD;

            return true;
        }
    }
    return false;
}
