// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Game/AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/Interface/Interactable.h"
#include "Player/AuraPlayerState.h"
#include "Tools/AuraCheatManager.h"
#include "UI/Widget/DamageFloatingTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;
    CheatClass = UAuraCheatManager::StaticClass();

    // 鼠标-按键行走: 初始化
    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AAuraPlayerController::ShowDamageFloatingText_Implementation(ACharacter* TargetCharacter,
                                                                  const FDamageFloatingTextProperty& Property)
{
    if (IsValid(TargetCharacter) && IsValid(DamageFloatingTextComponentClass))
    {
        // 对于动态创建的组件, 需要手动注册
        UDamageFloatingTextComponent* DamageTextComp = NewObject<UDamageFloatingTextComponent>(
            TargetCharacter, DamageFloatingTextComponentClass);
        DamageTextComp->RegisterComponent();
        // 为了只在敌人受击时的位置处显示伤害数字, 需要附加并脱离敌人的根组件
        DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageTextComp->SetDamageValue(Property);
    }
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
    AutoRunToDestination();
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
    
#if WITH_EDITOR
    // 开启作弊
    EnableCheats();
#endif
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

    // 通过WASD移动时强行打断鼠标移动
    bIsAutoRun = false;
}

void AAuraPlayerController::CursorTrace()
{
    // 只有 Visibility Channel 为 Blocked 的才能被指针发现
    GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, CursorHit);
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

void AAuraPlayerController::AutoRunToDestination()
{
    if (APawn* ControlledPawn = GetPawn();
        ControlledPawn && bIsAutoRun)
    {
        // 寻找导航曲线Spline上距玩家最近的点, 让玩家朝曲线上该点的切线方向寻路
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),
                                                                                    ESplineCoordinateSpace::World);
        const FVector AutoRunDir = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(AutoRunDir);

        if (const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
            DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bIsAutoRun = false;
        }
    }
}

TObjectPtr<UAuraAbilitySystemComponent> AAuraPlayerController::InitAndGetAuraASC()
{
    if (!IsValid(AuraASC))
    {
        AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
    }

    return AuraASC;
}

void AAuraPlayerController::AbilityInputTagOnPressed(FGameplayTag InputTag)
{
    if (InitAndGetAuraASC() == nullptr)
    {
        return;
    }

    // 鼠标右键按下时的逻辑, 例如按键行走
    if (InputTag.MatchesTagExact(AuraGameplayTags::Input::Mouse::RMB))
    {
        bIsTargeting = CurrTracedActor ? true : false;
        bIsAutoRun = false;
    }
}

void AAuraPlayerController::AbilityInputTagOnReleased(FGameplayTag InputTag)
{
    if (InitAndGetAuraASC() == nullptr)
    {
        return;
    }

    // 鼠标右键松开时的逻辑, 例如按键行走
    if (InputTag.MatchesTagExact(AuraGameplayTags::Input::Mouse::RMB))
    {
        if (bIsTargeting)
        {
            // 如果此时指向Actor, 就执行按键绑定为鼠标右键的GA
            InitAndGetAuraASC()->AbilityInputTagOnReleased(InputTag);
        }
        else
        {
            // 按键行走: 点按相关逻辑
            if (APawn* ControlledPawn = GetPawn();
                ControlledPawn && MousePressTime <= ShortPressThreshold)
            {
                // 获取导航路径
                if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
                        this,
                        ControlledPawn->GetActorLocation(),
                        CachedDestination);
                    NavPath && !NavPath->PathPoints.IsEmpty())
                {
                    // 将导航路径点导入Spline曲线中
                    Spline->ClearSplinePoints();
                    for (const FVector& NavPathPoint : NavPath->PathPoints)
                    {
                        Spline->AddSplinePoint(NavPathPoint, ESplineCoordinateSpace::Type::World);
                        // DrawDebugSphere(GetWorld(), NavPathPoint, 8.f, 8, FColor::Green, false, 5.f);
                    }
                    CachedDestination = NavPath->PathPoints.Last();
                }
                // 开启点按寻路
                bIsAutoRun = true;
            }
            MousePressTime = 0.f;
            bIsTargeting = false;
        }

        return;
    }

    InitAndGetAuraASC()->AbilityInputTagOnReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagOnHeld(FGameplayTag InputTag)
{
    if (InitAndGetAuraASC() == nullptr)
    {
        return;
    }

    // 鼠标右键按住时的逻辑, 例如按键行走
    if (InputTag.MatchesTagExact(AuraGameplayTags::Input::Mouse::RMB))
    {
        if (bIsTargeting)
        {
            // 如果此时指向Actor, 就执行按键绑定为鼠标右键的GA
            InitAndGetAuraASC()->AbilityInputTagOnHeld(InputTag);
        }
        else
        {
            // 按键行走: 长按相关逻辑
            MousePressTime += GetWorld()->GetDeltaSeconds();

            if (CursorHit.bBlockingHit)
            {
                CachedDestination = CursorHit.ImpactPoint;
            }

            if (APawn* ControlledPawn = GetPawn())
            {
                const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
                ControlledPawn->AddMovementInput(WorldDirection);
            }
        }

        return;
    }

    // 其他按键按住时的逻辑
    InitAndGetAuraASC()->AbilityInputTagOnHeld(InputTag);
}

void AAuraPlayerController::ServerAddXpForDebug_Implementation(int32 Amount)
{
    AAuraPlayerState* AuraPS = CastChecked<AAuraPlayerState>(PlayerState);
    AuraPS->AddXp(Amount);
}
