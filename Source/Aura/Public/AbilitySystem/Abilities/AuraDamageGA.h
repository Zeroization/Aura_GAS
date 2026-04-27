// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGA.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGA : public UAuraGameplayAbility
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraGA|Properties|Damage")
    TMap<FGameplayTag, FScalableFloat> DamageElemToValue;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraGA|Properties|Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;
};
