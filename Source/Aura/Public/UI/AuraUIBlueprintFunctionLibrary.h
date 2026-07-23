// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraUIBlueprintFunctionLibrary.generated.h"

struct FGameplayTag;
struct FInputTagDisplayRow;
/**
 * 
 */
UCLASS()
class AURA_API UAuraUIBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Aura UI|Lookup")
    static FText GetDisplayNameByInputTag(UDataTable* InputTagDisplayDataTable, const FGameplayTag& InputTag);
};
