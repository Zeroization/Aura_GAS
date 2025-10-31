// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	EEAP_ApplyOnBeginOverlap UMETA(DisplayName = "ApplyOnBeginOverlap"),
	EEAP_ApplyOnEndOverlap UMETA(DisplayName = "ApplyOnEndOverlap"),
	EEAP_DoNotApply UMETA(DisplayName = "DoNotApply")
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	EERP_RemoveOnEndOverlap UMETA(DisplayName = "RemoveOnEndOverlap"),
	EERP_DoNotRemove UMETA(DisplayName = "DoNotRemove")
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	// Instant GE: 一般会永久改变BaseValue
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	TSubclassOf<UGameplayEffect> InstantGEClass;

	// Has Duration/Infinite GE: 一般会改变CurrentValue, 并在时间到后撤回修改
	// 可通过修改Period变成周期对BaseValue修改的GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	TSubclassOf<UGameplayEffect> HasDurationGEClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	TSubclassOf<UGameplayEffect> InfiniteGEClass;

	// GE应用的处理方式
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	EEffectApplicationPolicy GEApplicationPolicy = EEffectApplicationPolicy::EEAP_DoNotApply;

	// GE移除的处理方式
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	EEffectRemovalPolicy GERemovalPolicy = EEffectRemovalPolicy::EERP_RemoveOnEndOverlap;

	// GE移除时是否销毁本对象
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	bool bDestroyOnGERemoval = false;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGEClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
};
