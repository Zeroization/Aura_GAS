// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "DamageFloatingTextComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageFloatingTextProperty
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float Damage = 0.f;

	UPROPERTY(BlueprintReadOnly, meta=(Bitflags))
	uint8 DamageTypeFlags = static_cast<uint8>(EAuraDamageType::None);
};

/**
 * 
 */
UCLASS()
class AURA_API UDamageFloatingTextComponent : public UWidgetComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void SetDamageValue(const FDamageFloatingTextProperty& Property);
};
