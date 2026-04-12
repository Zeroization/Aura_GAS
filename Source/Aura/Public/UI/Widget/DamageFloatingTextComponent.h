// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageFloatingTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageFloatingTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetDamageValue(float Damage);
};
