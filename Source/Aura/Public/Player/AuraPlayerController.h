// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	/// Begin Category: Input <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	/// End Category: Input <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
};
