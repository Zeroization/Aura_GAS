// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Game/AuraGameplayTags.h"
#include "ExecCalc_Damage.generated.h"

struct AuraDamageAttributeStatics
{
    // Attribute捕获声明宏, 用于快速声明Attribute捕获相关变量
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitBonusDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToCaptureDefs;
    
    AuraDamageAttributeStatics()
    {
        // Attribute捕获定义宏, 参数: (AttributeSet类, Attribute名, 捕获来源Target/Source, 是否Snapshot(ture/false))
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::Armor, ArmorDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::ArmorPenetration, ArmorPenetrationDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::BlockChance, BlockChanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::CriticalHitChance, CriticalHitChanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::CriticalHitResistance, CriticalHitResistanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitBonusDamage, Source, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::CriticalHitBonusDamage, CriticalHitBonusDamageDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::ElemResistance::Fire, FireResistanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::ElemResistance::Lightning, LightningResistanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::ElemResistance::Arcane, ArcaneResistanceDef);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
        TagToCaptureDefs.Add(AuraGameplayTags::Attribute::Secondary::ElemResistance::Physical, PhysicalResistanceDef);
    }

    // 单例方法
    static AuraDamageAttributeStatics& Get()
    {
        static AuraDamageAttributeStatics Instance;
        return Instance;
    }
};

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

public:
    UExecCalc_Damage();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
