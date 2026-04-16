// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageFloatingTextComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageFloatingTextProperty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsBlockedHit = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCriticalHit = false;
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
	void SetDamageValue(float Damage, const FDamageFloatingTextProperty& Property);
};
