// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputTagDisplayRow.generated.h"

USTRUCT(BlueprintType)
struct FInputTagDisplayRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag InputTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;
};
