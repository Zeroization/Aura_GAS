// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameplayTags.h"
#include "Interaction/Interface/CombatInterface.h"


UExecCalc_Damage::UExecCalc_Damage()
{
    // 将定义好的Attribute捕获对象添加到本类的捕获列表中
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().ArmorDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().BlockChanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().CriticalHitResistanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().CriticalHitBonusDamageDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().FireResistanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().LightningResistanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().ArcaneResistanceDef);
    RelevantAttributesToCapture.Add(AuraDamageAttributeStatics::Get().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    // 获取必要信息
    const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();
    FGameplayEffectContextHandle GEContextHandle = GESpec.GetContext();
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* SourceAvatarActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatarActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    TScriptInterface<ICombatInterface> SourceCombatInterface = (SourceAvatarActor && SourceAvatarActor->Implements<UCombatInterface>())
                                                                   ? TScriptInterface<ICombatInterface>(SourceAvatarActor)
                                                                   : nullptr;
    TScriptInterface<ICombatInterface> TargetCombatInterface = (TargetAvatarActor && TargetAvatarActor->Implements<UCombatInterface>())
                                                                   ? TScriptInterface<ICombatInterface>(TargetAvatarActor)
                                                                   : nullptr;

    // 设置捕捉Attribute需要的参数
    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParameters.TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();

    // 通过SetByCaller获取Damage
    float Damage = 0.f;
    for (const auto& [ElemType, ResistanceType] : AuraGameplayTags::Damage::ElemType::GetElemTypeToResistanceMap())
    {
        float ElemResistance = 0.f;
        FGameplayEffectAttributeCaptureDefinition ResistanceDef = AuraDamageAttributeStatics::Get().TagToCaptureDefs[ResistanceType];
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef, EvaluateParameters, ElemResistance);
        ElemResistance = FMath::Clamp(ElemResistance, 0.f, 100.f);
     
        float ElemDamage = GESpec.GetSetByCallerMagnitude(ElemType, false, 0.f);
        ElemDamage *= (100.f - ElemResistance) / 100.f;
        
        Damage += ElemDamage;
    }

#pragma region 伤害计算: BlockChance部分
    // 捕捉Target的BlockChance: 用于判断是否成功格挡, 是则Damage减半
    float TargetBlockChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().BlockChanceDef, EvaluateParameters,
                                                               TargetBlockChance);
    TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

    const bool bSuccessfulBlock = FMath::RandRange(1, 100) <= TargetBlockChance;
    Damage = bSuccessfulBlock ? Damage * 0.5f : Damage;
    UAuraAbilitySystemLibrary::SetIsBlockedHit(GEContextHandle, bSuccessfulBlock);
#pragma endregion

#pragma region 伤害计算: Armor和ArmorPenetration部分
    // 捕捉Target的Armor: 按一定百分比忽略Damage部分值
    float TargetArmor = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().ArmorDef, EvaluateParameters, TargetArmor);
    TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

    // 捕捉Source的ArmorPenetration: 按一定的百分比忽略TargetArmor部分值
    float SourceArmorPenetration = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().ArmorPenetrationDef, EvaluateParameters,
                                                               SourceArmorPenetration);
    SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

    // 获取CurveTable上的相关计算系数
    UCharacterClassInfo* EnemyCharacterClassInfo = UAuraAbilitySystemLibrary::GetEnemyCharacterClassInfo(SourceAvatarActor);
    FRealCurve* ArmorPenetrationCoefCurve = EnemyCharacterClassInfo->DamageCalcCoefficientTable->FindCurve(FName("ArmorPenetration"), {});
    float ArmorPenetrationCoef = (SourceCombatInterface && ArmorPenetrationCoefCurve)
                                     ? ArmorPenetrationCoefCurve->Eval(SourceCombatInterface->GetActorLevel())
                                     : 0.5f;
    FRealCurve* EffectiveArmorCoefCurve = EnemyCharacterClassInfo->DamageCalcCoefficientTable->FindCurve(FName("EffectiveArmor"), {});
    float EffectiveArmorCoef = (TargetCombatInterface && EffectiveArmorCoefCurve)
                                   ? EffectiveArmorCoefCurve->Eval(TargetCombatInterface->GetActorLevel())
                                   : 0.333f;

    float EffectiveTargetArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoef) / 100.f;
    Damage = Damage * (100.f - EffectiveTargetArmor * EffectiveArmorCoef) / 100.f;
#pragma endregion

#pragma region 伤害计算: CriticalHit部分
    // 捕捉Source的CriticalHitChance: 暴击率, 暴击将造成双倍伤害+暴击补偿伤害
    float SourceCriticalHitChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().CriticalHitChanceDef, EvaluateParameters,
                                                               SourceCriticalHitChance);
    SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

    // 捕捉Target的CriticalHitResistance: 百分比减少来自敌人攻击的暴击率
    float TargetCriticalHitResistance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().CriticalHitResistanceDef,
                                                               EvaluateParameters, TargetCriticalHitResistance);
    TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

    // 捕捉Source的CriticalHitBonusDamage: 暴击补偿伤害
    float SourceCriticalHitBonusDamage = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageAttributeStatics::Get().CriticalHitBonusDamageDef,
                                                               EvaluateParameters, SourceCriticalHitBonusDamage);
    SourceCriticalHitBonusDamage = FMath::Max<float>(SourceCriticalHitBonusDamage, 0.f);

    FRealCurve* CriticalHitResCoefCurve = EnemyCharacterClassInfo->DamageCalcCoefficientTable->
                                                                   FindCurve(FName("CriticalHitResistance"), {});
    float CriticalHitResCoef = (TargetCombatInterface && CriticalHitResCoefCurve)
                                   ? CriticalHitResCoefCurve->Eval(TargetCombatInterface->GetActorLevel())
                                   : 0.15f;
    const float EffectiveCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResCoef,
                                                               0.f);
    const bool bSuccessfulCriticalHit = FMath::RandRange(1, 100) <= EffectiveCriticalHitChance;
    Damage = bSuccessfulCriticalHit ? Damage * 2.f + SourceCriticalHitBonusDamage : Damage;
    UAuraAbilitySystemLibrary::SetIsCriticalHit(GEContextHandle, bSuccessfulCriticalHit);
#pragma endregion

    // 修改IncomingDamage属性的值为Damage, 并应用
    Damage = FMath::Max<float>(Damage, 0.f);
    const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
