// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AuraUIBlueprintFunctionLibrary.h"

#include "GameplayTagContainer.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"
#include "Tools/AuraDataTableFunctionLibrary.h"
#include "UI/Data/TableRows/InputTagDisplayRow.h"

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
