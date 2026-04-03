// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/Actor/AuraProjectile.h"
#include "Interaction/Interface/CombatInterface.h"
#include "Game/AuraGameplayTags.h"


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

		// 赋予投射物相关GE
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
		const FGameplayEffectSpecHandle GESpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(),
		                                                                           SourceASC->MakeEffectContext());
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		GESpecHandle.Data->SetSetByCallerMagnitude(AuraGameplayTags::Attribute::Meta::IncomingDamage.GetTag(),
		                                           ScaledDamage);

		Projectile->DamageEffectSpecHandle = GESpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
