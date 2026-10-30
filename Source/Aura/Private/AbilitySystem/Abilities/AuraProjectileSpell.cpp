// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"
#include "Interaction/Actor/AuraProjectile.h"
#include "Interaction/Interface/CombatInterface.h"
#include "Game/AuraGameplayTags.h"


FString UAuraProjectileSpell::GetDescription(int32 AbilityLevel, UObject* WorldContextObject)
{
    if (UAuraAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(WorldContextObject))
    {
        FAuraAbilityData Data;
        FStringFormatNamedArguments Args;
        if (AbilityInfo->FindAbilityDataByTag(Data, AbilityTags.GetByIndex(0)) &&
            GetDescFormatArgs(Args, AbilityLevel))
        {
            return FString::Format(*Data.DescTemplate, Args);
        }
    }
    return TEXT("如果你看到这串字符, 说明UAuraProjectileSpell::GetDescription中出现了错误.");
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 NextLevel, UObject* WorldContextObject)
{
    if (UAuraAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(WorldContextObject))
    {
        FAuraAbilityData Data;
        FStringFormatNamedArguments Args;
        if (AbilityInfo->FindAbilityDataByTag(Data, AbilityTags.GetByIndex(0)) &&
            GetDescFormatArgs(Args, NextLevel))
        {
            return FString::Format(*Data.NextLvlDescTemplate, Args);
        }
    }
    return TEXT("如果你看到这串字符, 说明UAuraProjectileSpell::GetDescription中出现了错误.");
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag)
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
        SpawnTransform.SetLocation(ICombatInterface::Execute_GetCombatSocketLocation(Owner, SocketTag));
        SpawnTransform.SetRotation(Rotation.Quaternion());

        AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
            ProjectileClass, SpawnTransform, Owner, Cast<APawn>(Owner),
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
        Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        Projectile->FinishSpawning(SpawnTransform);
    }
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UAuraProjectileSpell::GetDescFormatArgs(FStringFormatNamedArguments& Args, int32 AbilityLevel)
{
    if (Super::GetDescFormatArgs(Args, AbilityLevel))
    {
        Args.Add(TEXT("_ProjectileNum"), FMath::Min(AbilityLevel, ProjectileMaxSpawnNum));
    }
    return true;
}
