// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

USTRUCT(BlueprintType)
struct FAuraWidgetControllerParams
{
	GENERATED_BODY()

	FAuraWidgetControllerParams() = default;

	FAuraWidgetControllerParams(AAuraPlayerController* InPlayerController, AAuraPlayerState* InPlayerState,
	                            UAuraAbilitySystemComponent* InAbilitySystemComponent,
	                            UAuraAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState),
		  AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAuraPlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AAuraPlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAuraAttributeSet> AttributeSet = nullptr;
};

/**
 * MVC UI 架构
 * Controller: 获取Model数据，提供给View UI
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FAuraWidgetControllerParams& InParams);

	virtual void BroadcastInitialValues();
	virtual void BindDelegateCallbackFunctions();

protected:
	/// Begin Category: Widget Controller >>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
	/// End Category: Widget Controller   >>>>>>>>>>>>>>>>>>>>>>>
};
