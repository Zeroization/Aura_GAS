// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "DisplayWidgetRow.generated.h"

class UAuraAbilityInfo;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FDisplayWidgetRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayMessage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UTexture2D> DisplayImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag Tag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UAuraUserWidget> DisplayWidget;
};
