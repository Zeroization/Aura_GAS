// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/// 触发GA的默认输入Tag
	UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Input")
	FGameplayTag StartupInputTag;
	
	/* GA可能用到的数值属性 */
	/// Damage: 造成的伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraGA|Properties")
	FScalableFloat Damage;
};
