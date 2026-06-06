// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

    AAuraProjectile();

protected:
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                              bool bFromSweep, const FHitResult& SweepResult);

    virtual void BeginPlay() override;
    virtual void Destroyed() override;

private:
    UPROPERTY(EditAnywhere, Category = "Aura Projectile")
    float ProjectileLifeSpan = 10.f;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;

    /* Begin: SFX */
    UPROPERTY(EditAnywhere, Category = "Aura Projectile|SFX")
    TObjectPtr<UAudioComponent> LoopSoundComponent;

    UPROPERTY(EditAnywhere, Category = "Aura Projectile|SFX")
    TObjectPtr<UNiagaraSystem> ImpactEffect;

    UPROPERTY(EditAnywhere, Category = "Aura Projectile|SFX")
    TObjectPtr<USoundBase> ImpactSound;

    // 客户端Destroyed()和OnSphereBeginOverlap()的调用顺序未知, 需要用该变量区分ImpactSFX到底播放了没有
    bool bIsImpactSFXPlayed = false;

    void PlayImpactSFX();
    /* End: SFX */
};
