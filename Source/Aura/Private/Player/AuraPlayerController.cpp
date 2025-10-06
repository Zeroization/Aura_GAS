// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 设置InputMappingContext
	checkf(IMC_AuraContext, TEXT("IMC_AuraContext is not set !"));
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_AuraContext, 0);
	}
	else
	{
		checkf(false, TEXT("Can't get UEnhancedInputLocalPlayerSubsystem !"));
	}

	// 设置鼠标指针相关
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}
