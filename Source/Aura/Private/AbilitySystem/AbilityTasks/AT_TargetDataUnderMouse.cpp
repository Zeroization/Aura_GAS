// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AT_TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"


UAT_TargetDataUnderMouse* UAT_TargetDataUnderMouse::CreateTargetDataUnderMouseProxy(UGameplayAbility* OwningAbility)
{
	UAT_TargetDataUnderMouse* MyObj = NewAbilityTask<UAT_TargetDataUnderMouse>(OwningAbility);
	// 在这里初始化一些值

	return MyObj;
}

void UAT_TargetDataUnderMouse::SendMouseCursorData()
{
	// 此作用域参与客户端网络预测
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// 通过PlayerController获取鼠标指向对象的位置
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// 存入TargetData, 并发送到服务器
	// 这里的new不必担心, 后续Handle.Add()会将其封装到共享指针中
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	// 客户端的预测广播, 为了减少延迟
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnValidDataGet.Broadcast(TargetDataHandle);
	}
}

void UAT_TargetDataUnderMouse::OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 告知ASC数据已被服务器接受并使用, 无需继续缓存下去
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// 服务器的确认广播, 确保数据正确和同步
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnValidDataGet.Broadcast(DataHandle);
	}
}

void UAT_TargetDataUnderMouse::Activate()
{
	const bool bIsClient = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsClient)
	{
		// 客户端: 发送TargetData
		SendMouseCursorData();
	}
	else
	{
		// 服务器: 监听TargetData
		FGameplayAbilitySpecHandle GASpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(GASpecHandle, ActivationPredictionKey)
		                      .AddUObject(this, &UAT_TargetDataUnderMouse::OnAbilityTargetDataSet);
		// 如果服务器监听操作比客户端发送操作慢, 就得立刻执行监听回调
		const bool bIsDelegateCalled = AbilitySystemComponent->
			CallReplicatedTargetDataDelegatesIfSet(GASpecHandle, ActivationPredictionKey);
		if (!bIsDelegateCalled)
		{
			// 监听回调没有被立即执行, 说明服务器还在等客户端的TargetData数据
			SetWaitingOnRemotePlayerData();
		}
	}
}
