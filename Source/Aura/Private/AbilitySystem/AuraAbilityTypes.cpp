// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilityTypes.h"

void FAuraGameplayEffectContext::SetIsBlockedHit(bool bInIsBlockedHit)
{
    bIsBlockedHit = bInIsBlockedHit;
}

bool FAuraGameplayEffectContext::GetIsBlockedHit() const
{
    return bIsBlockedHit;
}

void FAuraGameplayEffectContext::SetIsCriticalHit(bool bInIsCriticalHit)
{
    bIsCriticalHit = bInIsCriticalHit;
}

bool FAuraGameplayEffectContext::GetIsCriticalHit() const
{
    return bIsCriticalHit;
}

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
    return StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
    uint32 RepBits = 0;

    // 序列化阶段: 通过按位或RepBits确认要存储多少数据
    if (Ar.IsSaving())
    {
#pragma region FGameplayEffectContext
        if (bReplicateInstigator && Instigator.IsValid())
        {
            RepBits |= 1 << 0;
        }
        if (bReplicateEffectCauser && EffectCauser.IsValid())
        {
            RepBits |= 1 << 1;
        }
        if (AbilityCDO.IsValid())
        {
            RepBits |= 1 << 2;
        }
        if (bReplicateSourceObject && SourceObject.IsValid())
        {
            RepBits |= 1 << 3;
        }
        if (Actors.Num() > 0)
        {
            RepBits |= 1 << 4;
        }
        if (HitResult.IsValid())
        {
            RepBits |= 1 << 5;
        }
        if (bHasWorldOrigin)
        {
            RepBits |= 1 << 6;
        }
#pragma endregion
        if (bIsBlockedHit)
        {
            RepBits |= 1 << 7;
        }
        if (bIsCriticalHit)
        {
            RepBits |= 1 << 8;
        }
    }
    // 让Archive知道要存储多少数据
    Ar.SerializeBits(&RepBits, 9);

    // 序列化/反序列化阶段: 通过按位与的结果存储/读取相应数据
#pragma region FGameplayEffectContext
    if (RepBits & (1 << 0))
    {
        // 对于一般类型, 使用<<即可
        Ar << Instigator;
    }
    if (RepBits & (1 << 1))
    {
        Ar << EffectCauser;
    }
    if (RepBits & (1 << 2))
    {
        Ar << AbilityCDO;
    }
    if (RepBits & (1 << 3))
    {
        Ar << SourceObject;
    }
    if (RepBits & (1 << 4))
    {
        // 对于数组类型, 需要使用该函数
        SafeNetSerializeTArray_Default<31>(Ar, Actors);
    }
    if (RepBits & (1 << 5))
    {
        // 特定类型拥有自己的NetSerialize()函数
        if (Ar.IsLoading())
        {
            if (!HitResult.IsValid())
            {
                HitResult = MakeShared<FHitResult>();
            }
        }
        HitResult->NetSerialize(Ar, Map, bOutSuccess);
    }
    if (RepBits & (1 << 6))
    {
        Ar << WorldOrigin;
        bHasWorldOrigin = true;
    }
    else
    {
        bHasWorldOrigin = false;
    }
#pragma endregion
    if (RepBits & (1 << 7))
    {
        Ar << bIsBlockedHit;
    }
    if (RepBits & (1 << 8))
    {
        Ar << bIsCriticalHit;
    }

#pragma region FGameplayEffectContext
    if (Ar.IsLoading())
    {
        AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
    }
#pragma endregion

    bOutSuccess = true;
    return true;
}

FAuraGameplayEffectContext* FAuraGameplayEffectContext::Duplicate() const
{
    FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
    *NewContext = *this;
    if (GetHitResult())
    {
        // Does a deep copy of the hit result
        NewContext->AddHitResult(*GetHitResult(), true);
    }
    return NewContext;
}
