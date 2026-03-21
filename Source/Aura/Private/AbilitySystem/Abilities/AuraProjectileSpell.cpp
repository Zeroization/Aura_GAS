// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Interaction/Actor/AuraProjectile.h"
#include "Interaction/Interface/CombatInterface.h"


void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	// 只有服务器才能生成投射物
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (!Owner->HasAuthority())
	{
		return;
	}

	// 生成投射物, 撞到物体后对它应用GE
	if (Owner->Implements<UCombatInterface>())
	{
		const TScriptInterface<ICombatInterface> CombatInterface = TScriptInterface<ICombatInterface>(Owner);
		FRotator Rotation = (TargetLocation - Owner->GetActorLocation()).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatInterface->GetProjectileSpawnLocation());
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform, Owner, Cast<APawn>(Owner),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: 赋予投射物相关GE

		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
