// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAuraDebuffParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag DebuffTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="触发该Debuff的概率", ClampMin=0.f, ClampMax=100.f))
    float Chance = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="每次触发该Debuff造成的伤害", ClampMin=0.f))
    float PerDamage = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="每次触发该Debuff的间隔(秒), 0.f为只触发一次", ClampMin=0.f))
    float Frequency = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="该Debuff的持续时间(秒), 负数为持续时间无限", ClampMin=-1.f))
    float Duration = 0.f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsSucceeded = false;

    bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

template <>
struct TStructOpsTypeTraits<FAuraDebuffParams> : TStructOpsTypeTraitsBase2<FAuraDebuffParams>
{
    enum
    {
        WithNetSerializer = true,
    };
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

    void SetIsBlockedHit(bool bInIsBlockedHit);
    bool GetIsBlockedHit() const;
    void SetIsCriticalHit(bool bInIsCriticalHit);
    bool GetIsCriticalHit() const;
    void SetIsDoTDamage(bool bInIsDoTDamage);
    bool GetIsDoTDamage() const;

    void ResetDebuffs(int32 NewSize = 0);
    void AddDebuff(const FAuraDebuffParams& DebuffParams);
    void SetDebuffSuccessful(const FGameplayTag& DebuffTag, bool bSuccessful = true);
    TArray<FAuraDebuffParams> GetDebuffs() const;
    bool TryGetDebuffParams(const FGameplayTag& DebuffTag, FAuraDebuffParams& OutDebuffParams) const;

    void SetDeathImpulse(const FVector& InDeathImpulse);
    FVector GetDeathImpulse() const;

    // 必须重写此方法, 用于为反射系统提供数据
    virtual UScriptStruct* GetScriptStruct() const override;
    // 必须重写此方法, 用于网络同步
    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
    // 必须重写此方法, 保证GameplayEffectContext在被复制时不丢失自定义数据, 且返回值类型为本结构体类型
    virtual FAuraGameplayEffectContext* Duplicate() const override;

protected:
    UPROPERTY()
    bool bIsBlockedHit = false;

    UPROPERTY()
    bool bIsCriticalHit = false;

    UPROPERTY()
    bool bIsDoTDamage = false;

    UPROPERTY()
    TArray<FAuraDebuffParams> Debuffs;

    UPROPERTY()
    FVector DeathImpulse = FVector::ZeroVector;
};

// 必须定义的结构体: 用于告诉引擎该结构体支持哪些底层行为(例如网络序列化, 拷贝等), 进而让引擎调用相关逻辑 
template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
    };
};

USTRUCT(BlueprintType)
struct FAuraDamageEffectParams
{
    GENERATED_BODY()

    FAuraDamageEffectParams() = default;

    UPROPERTY()
    TObjectPtr<UObject> WorldContextObject;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceASC;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetASC;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY()
    TMap<FGameplayTag, float> DamageElemTypeToBaseDamage;

    UPROPERTY()
    TArray<FAuraDebuffParams> Debuffs;

    UPROPERTY()
    float AbilityLevel = 1.f;

    UPROPERTY()
    float DeathImpulseMagnitude = 0.f;

    UPROPERTY()
    FVector DeathImpulse = FVector::ZeroVector;
};
