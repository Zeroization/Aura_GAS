// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/DataAssets/CharacterClassInfo.h"
#include "AbilitySystem/Data/DataAssets/CharacterLevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
    GetCapsuleComponent()->SetGenerateOverlapEvents(false);

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

    /// Begin Category: VFX <<<<<<<<<<<<<<
    LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
    LevelUpNiagaraComponent->SetupAttachment(RootComponent);
    LevelUpNiagaraComponent->bAutoActivate = false;
    /// End Category: VFX <<<<<<<<<<<<<<<<
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

int32 AAuraCharacter::GetActorLevel_Implementation()
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(AuraPlayerState, TEXT("[%hs] AuraPlayerState is nullptr!"), __FUNCTION__);
    return AuraPlayerState->GetPlayerLevel();
}

ECharacterClass AAuraCharacter::GetCharacterClassEnum_Implementation()
{
    // 目前主角只有这一个职业
    return ECharacterClass::ECC_Elementalist;
}

void AAuraCharacter::PlayerAddXp_Implementation(int32 InXp)
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    AuraPlayerState->AddXp(InXp);
}

void AAuraCharacter::PlayerAddLevel_Implementation(int32 InLevel)
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    AuraPlayerState->AddLevel(InLevel);
}

void AAuraCharacter::PlayerAddAttributePoint_Implementation(int32 InAttributePoint)
{
    // TODO
}

void AAuraCharacter::PlayerAddSkillPoint_Implementation(int32 InSkillPoint)
{
    // TODO
}

int32 AAuraCharacter::PlayerGetXp_Implementation() const
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    return AuraPlayerState->GetXp();
}

int32 AAuraCharacter::PlayerGetLevelByXp_Implementation(int32 InXp) const
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    return AuraPlayerState->AuraLevelUpInfo->GetLevelByXP(InXp);
}

int32 AAuraCharacter::PlayerGetAttributePointReward_Implementation(int32 InLevel) const
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    return AuraPlayerState->AuraLevelUpInfo->LevelUpInfos[InLevel].AttributePointReward;
}

int32 AAuraCharacter::PlayerGetSkillPointReward_Implementation(int32 InLevel) const
{
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));

    return AuraPlayerState->AuraLevelUpInfo->LevelUpInfos[InLevel].SkillPointReward;
}

void AAuraCharacter::PlayerOnLevelUp_Implementation()
{
    Multicast_PlayLevelUpVFX();
}

void AAuraCharacter::InitAbilitySystem()
{
    Super::InitAbilitySystem();

    // 1. 初始化ASC
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
    checkf(IsValid(AuraPlayerState), TEXT("Can't get AuraPlayerState !!!"));
    AuraAbilitySystemComponent = CastChecked<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
    AuraAbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
    AuraAbilitySystemComponent->OnAbilityActorInfoSet();

    // 2. 初始化AttributeSet和Attributes
    AuraAttributeSet = CastChecked<UAuraAttributeSet>(AuraPlayerState->GetAttributeSet());
    InitDefaultAttributes();

    // 3. 初始化GA
    GrantCharacterStartupAbilities();
}

void AAuraCharacter::InitPlayerHUD()
{
    if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
        {
            AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
            checkf(AuraPlayerState, TEXT("Can't get AuraPlayerState !!!"));

            AuraHUD->InitHUD(AuraPlayerController, AuraPlayerState, AuraAbilitySystemComponent, AuraAttributeSet);
        }
    }
}

void AAuraCharacter::Multicast_PlayLevelUpVFX_Implementation() const
{
    if (IsValid(LevelUpNiagaraComponent))
    {
        // 将特效正对摄像机
        const FVector CameraLocation = CameraComponent->GetComponentLocation();
        const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
        const FRotator ToCamaraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
        LevelUpNiagaraComponent->SetWorldRotation(ToCamaraRotation);

        LevelUpNiagaraComponent->Activate(true);
    }
}
