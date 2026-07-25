// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MaxHealthMMC.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Interaction/Interface/CombatInterface.h"

UMaxHealthMMC::UMaxHealthMMC()
{
	// 1.初始化要捕获的Attribute信息:
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	// 2.将该Attribute信息加入要捕获的Attribute列表中
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMaxHealthMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取Source Actor和Target Actor的GameplayTags待用
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 用于获取参考Attributes值函数的参数
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	// 获取目标Attribute值
	float TargetVigorValue = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvalParams, TargetVigorValue);

	// 自定义计算: MaxHealth = 40.0 + 2.0 * Vigor + 10.0 * PlayerLevel
	TargetVigorValue = FMath::Max<float>(TargetVigorValue, 0.f);
	float MaxHealth = 0.f;
	if (UObject* Player = Spec.GetContext().GetSourceObject();
		Player && Player->Implements<UCombatInterface>())
	{
		const TScriptInterface<ICombatInterface> PlayerCombatInterface = TScriptInterface<ICombatInterface>(Player);
		const int32 PlayerLevel = PlayerCombatInterface->GetActorLevel();
		MaxHealth = 40.f + 2.0f * TargetVigorValue + 10.f * PlayerLevel;
	}
	else
	{
		UE_LOG(LogAuraGame, Error, TEXT("[%hs] Target actor is not player (Not implements ICombatInterface)"), __FUNCTION__);
	}

	return MaxHealth;
}
