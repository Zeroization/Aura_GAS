// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	/// 鼠标指针追踪相关
	/// Ps: 要声明实现某接口的Actor变量, 类型是TScriptInterface<I接口名>
	TScriptInterface<IInteractable> LastTracedActor;
	TScriptInterface<IInteractable> CurrTracedActor;
	void CursorTrace();

	/// Ability System
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	void AbilityInputTagOnPressed(FGameplayTag InputTag);
	void AbilityInputTagOnReleased(FGameplayTag InputTag);
	void AbilityInputTagOnHeld(FGameplayTag InputTag);
	/// Ability System
};
