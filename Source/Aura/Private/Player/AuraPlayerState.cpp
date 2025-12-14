// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"


AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	// 对于玩家, 需要将 AbilitySystem 和 AttributeSet 放到 PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	/**
	 * 有关ReplicationMode:
	 *		1. Full:	适用于极少数玩家场景(单机/局域网), Gameplay Effects会被同步到所有客户端中;
	 *		2. Mixed:	适用于多人游戏, 玩家控制的, Gameplay Effects只会被同步到玩家拥有的客户端中,
	 *					Gameplay Cues 和 GameplayTags 会被同步到所有客户端中;
	 *		3. Minimal:	适用于多人游戏, AI控制的, Gameplay Effects不会被同步, Gameplay Cues 和
	 *					GameplayTags 会被同步到所有客户端中.
	 */
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
}
