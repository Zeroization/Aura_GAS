// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* InTargetActor, const FEffectActorGE& InEffectActorGE)
{
	if (InTargetActor->Implements<UAbilitySystemInterface>())
	{
		checkf(InEffectActorGE.GEClass, TEXT("[%hs] GEClass is empty, please fill out in editor!"), __FUNCTION__);

		// 1. 获取Target自己的ASC
		const TScriptInterface<IAbilitySystemInterface> ASCInterface = TScriptInterface<IAbilitySystemInterface>(
			InTargetActor);
		UAbilitySystemComponent* TargetActorAsc = ASCInterface->GetAbilitySystemComponent();
		// 2. 创建GEContextHandle(封装GEContext + 工具函数)
		FGameplayEffectContextHandle GEContextHandle = TargetActorAsc->MakeEffectContext();
		GEContextHandle.AddSourceObject(this);
		// 3. 创建GESpecHandle(封装GESpec + 工具函数)
		const FGameplayEffectSpecHandle GESpecHandle = TargetActorAsc->MakeOutgoingSpec(
			InEffectActorGE.GEClass, 1.f, GEContextHandle);
		// 4. 对Target自己的ASC施加GE
		const FActiveGameplayEffectHandle ActiveGEHandle = TargetActorAsc->ApplyGameplayEffectSpecToSelf(
			*GESpecHandle.Data);

		// 5. 对于Infinite类GE, 需要存储激活它的FActiveGameplayEffectHandle, 以便后续移除它
		if (GESpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
			InEffectActorGE.GERemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
		{
			ActiveInfiniteGEHandles.Add(ActiveGEHandle, TargetActorAsc);
		}
	}

	// 获取ASC的另一个方法:
	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
}

void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	for (const auto& EffectActorGE : EffectActorGEs)
	{
		if (EffectActorGE.GEApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnBeginOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectActorGE);
		}
		
		// 真正的销毁是在OnEndOverlap()中实现, 这里先取消可见性吧
		if (EffectActorGE.bDestroyOnEndOverlap)
		{
			GetRootComponent()->SetVisibility(false, true);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (const auto& EffectActorGE : EffectActorGEs)
	{
		if (EffectActorGE.GEApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectActorGE);
		}
		if (EffectActorGE.GERemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC))
			{
				return;
			}

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (auto& [InfiniteGEHandle, ASC] : ActiveInfiniteGEHandles)
			{
				if (TargetASC == ASC)
				{
					TargetASC->RemoveActiveGameplayEffect(InfiniteGEHandle, 1);
					HandlesToRemove.Add(InfiniteGEHandle);
				}
			}
			for (auto& Handle : HandlesToRemove)
			{
				ActiveInfiniteGEHandles.Remove(Handle);
			}
		}

		if (EffectActorGE.bDestroyOnEndOverlap)
		{
			Destroy();
		}
	}
}
