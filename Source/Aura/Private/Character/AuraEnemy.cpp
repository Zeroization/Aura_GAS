// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"


void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
    if (IsValid(InCombatTarget))
    {
        CombatTarget = InCombatTarget;
    }
    else
    {
        UE_LOG(LogAuraGame, Warning, TEXT("[%hs] Invalid CombatTarget! Character: %s"), __FUNCTION__, *GetNameSafe(this));
    }
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
    return CombatTarget;
}

AAuraEnemy::AAuraEnemy()
{
    // 初始化Character Mesh的Visibility碰撞通道为Block, 方便鼠标追踪
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);

    // 敌人平滑转弯
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    /* Begin: Ability System */
    // 对于敌人, 将 AbilitySystem 和 AttributeSet 放到这里即可
    AuraAbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AuraAbilitySystemComponent->SetIsReplicated(true);
    AuraAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AuraAttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
    /* End: Ability System */

    /* Begin: UI */
    HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
    HealthBar->SetupAttachment(GetRootComponent());
    /* End: UI */

    /* Begin: AI */
    // 通过SpawnActor()生成时自动生成对应Controller
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    /* End: AI */
}

void AAuraEnemy::OnGEHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    bDoHitReacting = NewCount > 0;
    GetCharacterMovement()->MaxWalkSpeed = bDoHitReacting ? 0.f : DefaultWalkSpeed;

    if (IsValid(AuraAIController))
    {
        AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsHitReacting", bDoHitReacting);
    }
}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();

    GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

    /* Begin: Ability System */
    InitAbilitySystem();
    UAuraAbilitySystemLibrary::GrantEnemyStartupAbilities(this, AuraAbilitySystemComponent, CharacterClass);
    // 绑定特定GameplayTag被添加/移除的回调
    AuraAbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::GE::HitReact.GetTag(), EGameplayTagEventType::NewOrRemoved)
                              .AddUObject(this, &AAuraEnemy::OnGEHitReactTagChanged);
    /* End: Ability System */

    /* Begin: UI */
    if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
    {
        AuraUserWidget->SetWidgetController(this);
    }

    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnHealthChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  // UE5.4 问题（UE5.5 已修复）
                                  // 原教程使用 Data.NewValue（BaseValue）作为 MaxHealth。
                                  // 但本项目中 MaxHealth 由 Infinite GE 修改，该类型 GE 只影响 CurrentValue，不修改 BaseValue。
                                  // 因此：
                                  // - BaseValue 始终为初始值（通常为 0）
                                  // - CurrentValue 才是实际生效的 MaxHealth
                                  // 而 GAMEPLAYATTRIBUTE_REPNOTIFY 在 OnRep 中传递的是 BaseValue，
                                  // 导致客户端读取到的 MaxHealth 为 0，从而血条计算错误。
                                  // 正确做法：使用 GetMaxHealth() 获取 CurrentValue。
                                  OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
                              });
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    /* End: UI */
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AuraAIController = Cast<AAuraAIController>(NewController);
    // 初始化黑板
    AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
    AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsHitReacting", false);
    AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsRangedAttacker", CharacterClass != ECharacterClass::ECC_Warrior);
    // 运行敌人行为树
    AuraAIController->RunBehaviorTree(BehaviorTree);
}

void AAuraEnemy::InitAbilitySystem()
{
    Super::InitAbilitySystem();

    AuraAbilitySystemComponent->InitAbilityActorInfo(this, this);
    AuraAbilitySystemComponent->OnAbilityActorInfoSet();

    InitDefaultAttributes();
}

void AAuraEnemy::InitDefaultAttributes() const
{
    UAuraAbilitySystemLibrary::InitEnemyDefaultAttributesByClass(this, CharacterClass, Level, AuraAbilitySystemComponent);
}

void AAuraEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(OUTLINE_COLOR_RED);

    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(OUTLINE_COLOR_RED);
}

void AAuraEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);
    Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Die()
{
    SetLifeSpan(DyingLifeSpan);
    if (AuraAIController)
    {
        AuraAIController->GetBlackboardComponent()->SetValueAsBool("IsDead", true);
    }

    Super::Die();
}

int32 AAuraEnemy::GetActorLevel()
{
    return Level;
}
