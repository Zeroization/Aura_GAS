// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/Interface/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 只有服务器才能生成投射物
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer)
	{
		return;
	}

	// 生成投射物, 撞到物体后对它应用GE
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (Owner->Implements<UCombatInterface>())
	{
		FTransform SpawnTransform;
		const TScriptInterface<ICombatInterface> CombatInterface = TScriptInterface<ICombatInterface>(Owner);
		SpawnTransform.SetLocation(CombatInterface->GetProjectileSpawnLocation());
		// TODO: SpawnTransform.SetRotation()

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform, Owner, Cast<APawn>(Owner),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		// TODO: 赋予投射物相关GE
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
