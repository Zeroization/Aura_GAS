// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/Interface/Interactable.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 设置InputMappingContext
	checkf(IMC_AuraContext, TEXT("IMC_AuraContext is not set !"));
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(IMC_AuraContext, 0);
	}

	// 设置鼠标指针相关
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 绑定Input Action
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityInputAction(InputConfig, this,
	                                           {
		                                           &ThisClass::AbilityInputTagOnPressed,
		                                           &ThisClass::AbilityInputTagOnReleased,
		                                           &ThisClass::AbilityInputTagOnHeld
	                                           });
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 通过YawRotation获取玩家正前方和正右方的方向向量
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Type::Y);

	// 让玩家移动
	APawn* ControlledPawn = GetPawn<APawn>();
	if (ensureMsgf(ControlledPawn, TEXT("Can't get controlled pawn !")))
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	// 只有 Visibility Channel 为 Blocked 的才能被指针发现
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastTracedActor = CurrTracedActor;
	CurrTracedActor = CursorHit.GetActor();

	// 处理Actor高亮逻辑
	if (LastTracedActor != CurrTracedActor)
	{
		if (LastTracedActor != nullptr)
		{
			LastTracedActor->UnHighlightActor();
		}

		if (CurrTracedActor != nullptr)
		{
			CurrTracedActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagOnPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Cyan,
	                                 FString::Printf(TEXT("[%hs]: Gameplay Input Tag: %s"), __FUNCTION__, *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagOnReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Cyan,
	                                 FString::Printf(TEXT("[%hs]: Gameplay Input Tag: %s"), __FUNCTION__, *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagOnHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Cyan,
	                                 FString::Printf(TEXT("[%hs]: Gameplay Input Tag: %s"), __FUNCTION__, *InputTag.ToString()));
}
