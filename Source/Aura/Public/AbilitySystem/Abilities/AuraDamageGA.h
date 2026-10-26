// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AuraDamageGA.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGA : public UAuraGameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void CauseDamage(AActor* TargetActor);

    /// 封装伤害计算(Damage + Debuff)相关的数据绑定逻辑, 方便给其他代码使用
    /// @param TargetActor GE的TargetActor
    /// @return 封装好的参数
    FAuraDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraGA|Properties|Damage")
    TMap<FGameplayTag, FScalableFloat> DamageElemToValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraGA|Properties|Damage")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraGA|Properties|Damage")
    TMap<FGameplayTag, FAuraDebuffParams> DebuffTagToParams;

    virtual bool GetDescFormatArgs(FStringFormatNamedArguments& Args, int32 AbilityLevel) override;
};
