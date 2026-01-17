// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	/// Begin Category: Camera <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	/// End Category: Camera <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	/// Begin: Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	/// End: Movement
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务端初始化
	InitAbilitySystem();
	InitPlayerHUD();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 客户端初始化
	InitAbilitySystem();
	InitPlayerHUD();
}

int32 AAuraCharacter::GetActorLevel()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("[%hs] AuraPlayerState is nullptr!"), __FUNCTION__);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("Can't get AuraPlayerState !!!"));
	AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
	AuraAbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AuraAbilitySystemComponent->OnAbilityActorInfoSet();

	AuraAttributeSet = CastChecked<UAuraAttributeSet>(AuraPlayerState->GetAttributeSet());

	InitDefaultAttributes();
}

void AAuraCharacter::InitPlayerHUD()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD());
		checkf(AuraHUD, TEXT("Can't get AuraHUD !!!"));
		AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
		checkf(AuraPlayerState, TEXT("Can't get AuraPlayerState !!!"));

		AuraHUD->InitHUD(AuraPlayerController, AuraPlayerState, AuraAbilitySystemComponent, AuraAttributeSet);
	}
}
