// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_BODY()

    void SetIsBlockedHit(bool bInIsBlockedHit);
    bool GetIsBlockedHit() const;
    void SetIsCriticalHit(bool bInIsCriticalHit);
    bool GetIsCriticalHit() const;

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
