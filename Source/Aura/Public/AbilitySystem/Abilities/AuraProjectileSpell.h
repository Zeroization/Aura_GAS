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

public:
    virtual FString GetDescription(int32 AbilityLevel, UObject* WorldContextObject) override;
    virtual FString GetNextLevelDescription(int32 NextLevel, UObject* WorldContextObject) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraGA|Properties|Projectile")
    TSubclassOf<AAuraProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraGA|Properties|Projectile")
    int32 ProjectileMaxSpawnNum = 5;

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag);

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual bool GetDescFormatArgs(FStringFormatNamedArguments& Args, int32 AbilityLevel) override;
};
