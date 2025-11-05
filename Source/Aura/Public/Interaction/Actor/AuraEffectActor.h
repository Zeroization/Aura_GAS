// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
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

USTRUCT(BlueprintType)
struct FEffectActorGE
{
	GENERATED_BODY()

	// Instant GE: 一般会永久改变BaseValue
	// Has Duration GE & Infinite GE: 一般会改变CurrentValue, 并在时间到后撤回修改
	// 可通过修改Period变成周期对BaseValue修改的GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GEClass;

	// GE应用的处理方式
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy GEApplicationPolicy = EEffectApplicationPolicy::EEAP_DoNotApply;

	// GE移除的处理方式
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy GERemovalPolicy = EEffectRemovalPolicy::EERP_DoNotRemove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GELevel = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDestroyOnEndOverlap = false;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Gameplay Effects")
	TArray<FEffectActorGE> EffectActorGEs;

	// 用于激活/移除Infinite类型GE的TMap
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, TObjectPtr<UAbilitySystemComponent>> ActiveInfiniteGEHandles;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* InTargetActor, const FEffectActorGE& InEffectActorGE);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
};
