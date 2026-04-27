// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGA.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 生成投射物的GA
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGA
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraGA|Properties|Projectile")
    TSubclassOf<AAuraProjectile> ProjectileClass;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectile(const FVector& TargetLocation);

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
