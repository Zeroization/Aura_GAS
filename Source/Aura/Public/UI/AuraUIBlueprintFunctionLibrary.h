// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetController/AuraWidgetController.h"
#include "AuraUIBlueprintFunctionLibrary.generated.h"

class AAuraHUD;
class UAuraSpellMenuWidgetController;
class UAuraAttributeMenuWidgetController;
class UAuraOverlayWidgetController;
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

#pragma region Widget Controller
    UFUNCTION(BlueprintPure, Category = "Aura UI|WidgetController", meta = (WorldContext="WorldContextObject"))
    static UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Aura UI|WidgetController", meta = (WorldContext="WorldContextObject"))
    static UAuraAttributeMenuWidgetController* GetAuraAttributeMenuWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Aura UI|WidgetController", meta = (WorldContext="WorldContextObject"))
    static UAuraSpellMenuWidgetController* GetAuraSpellMenuWidgetController(const UObject* WorldContextObject);
#pragma endregion

private:
    static bool MakeAuraWidgetControllerParam(const UObject* WorldContextObject, FAuraWidgetControllerParams& OutWCParam,
                                              AAuraHUD*& OutAuraHUD);
};
