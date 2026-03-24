// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	// 初始化Character Mesh的Visibility碰撞通道为Block, 方便鼠标追踪
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();

	/* Begin: UI */
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                          .AddLambda([this](const FOnAttributeChangeData& Data)
	                          {
		                          OnHealthChanged.Broadcast(Data.NewValue);
		                          // TODO: 多人模式下, 客户端敌人血条显示有问题, 目前仅能这样勉强解决
		                          OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	                          });
	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                          .AddLambda([this](const FOnAttributeChangeData& Data)
	                          {
		                          OnMaxHealthChanged.Broadcast(Data.NewValue);
	                          });
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	/* End: UI */
}

void AAuraEnemy::InitAbilitySystem()
{
	Super::InitAbilitySystem();

	AuraAbilitySystemComponent->InitAbilityActorInfo(this, this);
	AuraAbilitySystemComponent->OnAbilityActorInfoSet();

	InitDefaultAttributes();
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

int32 AAuraEnemy::GetActorLevel()
{
	return Level;
}
