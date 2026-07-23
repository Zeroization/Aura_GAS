// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MaxManaMMC.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Interaction/Interface/CombatInterface.h"


UMaxManaMMC::UMaxManaMMC()
{
	// 1.初始化要捕获的Attribute信息:
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
	// 2.将该Attribute信息加入要捕获的Attribute列表中
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMaxManaMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取Source Actor和Target Actor的GameplayTags待用
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 用于获取参考Attributes值函数的参数
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	// 获取目标Attribute值
	float TargetIntelligenceValue = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvalParams, TargetIntelligenceValue);

	// 自定义计算: MaxMana = 50.0 + 2.5 * Intelligence + 15.0 * PlayerLevel
	TargetIntelligenceValue = FMath::Max<float>(TargetIntelligenceValue, 0.f);
	float MaxMana = 0.f;
	if (UObject* Player = Spec.GetContext().GetSourceObject();
		Player && Player->Implements<UCombatInterface>())
	{
		const TScriptInterface<ICombatInterface> PlayerCombatInterface = TScriptInterface<ICombatInterface>(Player);
		const int32 PlayerLevel = PlayerCombatInterface->GetActorLevel();
		MaxMana = 50.f + 2.5f * TargetIntelligenceValue + 15.f * PlayerLevel;
	}
	else
	{
		UE_LOG(LogAuraGame, Error, TEXT("[%hs] Target actor is not player (Not implements ICombatInterface)"), __FUNCTION__);
	}

	return MaxMana;
}
