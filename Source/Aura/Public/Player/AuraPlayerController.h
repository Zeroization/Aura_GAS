// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FDamageFloatingTextProperty;
class UDamageFloatingTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
struct FGameplayTag;
class UAuraInputConfig;
class IInteractable;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

/**
 *   
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamageFloatingText(float DamageAmount, ACharacter* TargetCharacter, const FDamageFloatingTextProperty& Property);

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Aura Player Controller|Input")
	TObjectPtr<UInputMappingContext> IMC_AuraContext;

	UPROPERTY(EditAnywhere, Category = "Aura Player Controller|Input")
	TObjectPtr<UInputAction> IA_MoveAction;

	void Move(const FInputActionValue& InputActionValue);

#pragma region UI
	UPROPERTY(EditDefaultsOnly, Category = "Aura Player Controller|UI")
	TSubclassOf<UDamageFloatingTextComponent> DamageFloatingTextComponentClass;
#pragma endregion

#pragma region 鼠标: 指针追踪 Cursor Trace
	FHitResult CursorHit;

	// Ps: 要声明实现某接口的Actor变量, 类型是TScriptInterface<I接口名>
	TScriptInterface<IInteractable> LastTracedActor;
	TScriptInterface<IInteractable> CurrTracedActor;

	void CursorTrace();
#pragma endregion

#pragma region 鼠标: 按键行走 Click To Move
	// 实现在Top-Down视角下, 玩家能通过鼠标点按让角色行走的功能.
	// 1. 鼠标短按: 鼠标朝终点短按, 角色会自动朝向终点前进, 路径为避开障碍物的曲线;
	// 2. 鼠标长按: 玩家朝鼠标指向方向持续行走;

	// 终点位置
	FVector CachedDestination;
	// 鼠标点按的持续时间, 用于识别长/短按
	float MousePressTime = 0.f;
	// 鼠标短按的最长持续时间
	UPROPERTY(EditDefaultsOnly, Category = "Aura Player Controller|Click To Move")
	float ShortPressThreshold = 0.5f;
	// 自动行走实际终点与理论终点的距离精度值
	UPROPERTY(EditDefaultsOnly, Category = "Aura Player Controller|Click To Move")
	float AutoRunAcceptanceRadius = 50.f;
	// 自动行走的导航路线
	UPROPERTY(VisibleAnywhere, Category = "Aura Player Controller|Click To Move")
	TObjectPtr<USplineComponent> Spline;
	// 判断当前是否是鼠标短按的自动行走状态
	bool bIsAutoRun = false;
	// 判断当前鼠标是否指向一个对象(例如敌人)
	bool bIsTargeting = false;

	void AutoRunToDestination();
#pragma endregion

#pragma region Ability System
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;

	UPROPERTY(EditDefaultsOnly, Category = "Aura Player Controller|Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	TObjectPtr<UAuraAbilitySystemComponent> InitAndGetAuraASC();

	void AbilityInputTagOnPressed(FGameplayTag InputTag);
	void AbilityInputTagOnReleased(FGameplayTag InputTag);
	void AbilityInputTagOnHeld(FGameplayTag InputTag);
#pragma endregion
};
