// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonGA.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonGA : public UAuraGameplayAbility
{
    GENERATED_BODY()

public:
    // 生成召唤物的类型
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    TArray<TSubclassOf<APawn>> MinionClassArray;

    // 生成召唤物的数量
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    int32 NumOfMinions = 5;

    // 离AvatarActor最近的生成距离
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    float MinSpawnDistance = 50.f;

    // 离AvatarActor最远的生成距离
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    float MaxSpawnDistance = 250.f;

    // 以AvatarActor正前方为准的扩散生成角度
    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    float SpawnSpreadAngle = 90.f;

    UPROPERTY(EditDefaultsOnly, Category = "AuraGA|Properties|Summoning")
    bool bEnableDebug = true;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<FVector> GetSpawnLocations();

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TSubclassOf<APawn> GetRandomMinionClass();
};
