// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Actor/AuraEffectActor.h"

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

void AAuraEffectActor::ApplyEffectToTarget(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGEClass)
{
	if (InTargetActor->Implements<UAbilitySystemInterface>())
	{
		checkf(InGEClass, TEXT("[%hs] Instant GEClass is empty, please fill out in editor!"), __FUNCTION__);
		
		// 1. 获取ASC
		TScriptInterface<IAbilitySystemInterface> ASCInterface = TScriptInterface<IAbilitySystemInterface>(InTargetActor);
		UAbilitySystemComponent* TargetActorASC = ASCInterface->GetAbilitySystemComponent();
		// 2. 创建GEContextHandle(封装GEContext + 工具函数)
		FGameplayEffectContextHandle GEContextHandle = TargetActorASC->MakeEffectContext();
		GEContextHandle.AddSourceObject(this);
		// 3. 创建GESpecHandle(封装GESpec + 工具函数)
		FGameplayEffectSpecHandle GESpecHandle = TargetActorASC->MakeOutgoingSpec(InGEClass, 1.f, GEContextHandle);
		// 4. 施加GE
		TargetActorASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data);
	}

	// 获取ASC的另一个方法:
	// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
}
