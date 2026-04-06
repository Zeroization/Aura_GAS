// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAuraAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAuraAttributeMenuWidgetController;
class UAuraOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
	static UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
	static UAuraAttributeMenuWidgetController* GetAuraAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
	static void InitEnemyDefaultAttributesByClass(const UObject* WorldContextObject, ECharacterClass EnemyClass, float Level,
	                                              UAuraAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
	static void GrantEnemyStartupAbilities(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC);
};
