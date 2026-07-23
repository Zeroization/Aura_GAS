// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeDelegate, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnCooldownChangeDelegate OnCooldownStart;

    UPROPERTY(BlueprintAssignable)
    FOnCooldownChangeDelegate OnCooldownEnd;

    UFUNCTION(BlueprintCallable, meta = (DisplayName ="Wait for Cooldown Change", BlueprintInternalUseOnly = "true"))
    static UWaitCooldownChange* WaitCooldownChangeProxy(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);

    UFUNCTION(BlueprintCallable)
    void EndTask();

protected:
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayTag CooldownTag;

    // 冷却Tag被移除时的回调, 可推断出此时冷却已经结束
    // 原型: FOnGameplayEffectTagCountChanged, const FGameplayTag, int32
    void OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

    // 冷却GE被添加时的回调, 可推断出此时冷却已经开始
    // 原型: FOnGameplayEffectAppliedDelegate, UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle
    void OnActiveGEAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);
};
