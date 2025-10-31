// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	// Instant GE: 一般会永久改变BaseValue
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> InstantGEClass;

	// Has Duration/Infinite GE: 一般会改变CurrentValue, 并在时间到后撤回修改
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> HasDurationGEClass;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGEClass);
};
