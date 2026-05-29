// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widget/DamageFloatingTextComponent.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
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
    static UCharacterClassInfo* GetEnemyCharacterClassInfo(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static void InitEnemyDefaultAttributesByClass(const UObject* WorldContextObject, ECharacterClass EnemyClass, float Level,
                                                  UAuraAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static void GrantEnemyStartupAbilities(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC,
                                           ECharacterClass CharacterClass);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static bool GetIsBlockedHit(const FGameplayEffectContextHandle& GEContextHandle);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static bool GetIsCriticalHit(const FGameplayEffectContextHandle& GEContextHandle);

    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, bool InIsBlockedHit);

    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, bool InIsCriticalHit);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Damage")
    static bool ContainsDamageTypeByProperty(const FDamageFloatingTextProperty& Property, EAuraDamageType DamageType);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Damage")
    static bool ContainsDamageTypeByFlags(uint8 DamageTypeFlags, EAuraDamageType DamageType);

    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|Environment Query", meta = (WorldContext="WorldContextObject"))
    static void QueryActorsInSphere(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                    const TArray<AActor*> ActorsToIgnore, float SphereRadius, const FVector& SphereOrigin);
};
