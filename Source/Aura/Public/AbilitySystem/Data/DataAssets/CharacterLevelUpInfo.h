// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterLevelUpInfo.generated.h"

USTRUCT(BLueprintType)
struct FAuraSingleLevelUpInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 LevelUpRequirement = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 AttributePointReward = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 SkillPointReward = 0;
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterLevelUpInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TArray<FAuraSingleLevelUpInfo> AuraLevelUpInfo;

    UFUNCTION(BlueprintCallable)
    int32 GetLevelByCurrentXP(int32 XP, bool bOutputLog = false) const;
};
