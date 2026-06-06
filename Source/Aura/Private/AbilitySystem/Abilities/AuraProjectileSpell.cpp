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
        FRotator Rotation = (TargetLocation - Owner->GetActorLocation()).Rotation();
        // Rotation.Pitch = 0.f;

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(
            ICombatInterface::Execute_GetCombatSocketLocation(Owner, AuraGameplayTags::CombatSocket::Weapon));
        SpawnTransform.SetRotation(Rotation.Quaternion());

        AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
            ProjectileClass, SpawnTransform, Owner, Cast<APawn>(Owner),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        // 赋予投射物相关GE
        const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
        FGameplayEffectContextHandle GEContextHandle = SourceASC->MakeEffectContext();
        GEContextHandle.SetAbility(this);
        GEContextHandle.AddSourceObject(Projectile);
        const FGameplayEffectSpecHandle GESpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(),
                                                                                   GEContextHandle);
        for (const auto& [DamageElemType, DamageValueTable] : DamageElemToValue)
        {
            const float ScaledDamage = DamageValueTable.GetValueAtLevel(GetAbilityLevel());
            GESpecHandle.Data->SetSetByCallerMagnitude(DamageElemType, ScaledDamage);
        }

        Projectile->DamageEffectSpecHandle = GESpecHandle;
        Projectile->FinishSpawning(SpawnTransform);
    }
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
