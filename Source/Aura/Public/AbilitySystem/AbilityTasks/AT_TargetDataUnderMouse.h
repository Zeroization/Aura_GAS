// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UAT_TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/* vv动态多播委托可以变成节点中的可执行引脚vv */

	/// 拿到目标位置信息时
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataDelegate OnValidDataGet;

	/* vv每个AbilityTask都得有这个static工厂函数, 该函数会作为可调用的蓝图节点显示vv */

	/// 获取鼠标指向对象的目标数据
	/// @param OwningAbility 拥有该AbilityTask的对象, 即GA自己
	/// @return 创建好的AbilityTask实例
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Target Data Under Mouse",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAT_TargetDataUnderMouse* CreateTargetDataUnderMouseProxy(UGameplayAbility* OwningAbility);

private:
	/// 客户端: 获取鼠标指向对象的准确位置, 发送给服务器
	void SendMouseCursorData();
	/// 服务器: 监听并接收位置信息, 在服务器上进行进一步处理的回调函数
	void OnAbilityTargetDataSet(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

	virtual void Activate() override;
};
