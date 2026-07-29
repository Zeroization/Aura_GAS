// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent)
    void PlayerAddXp(int32 InXp);

    UFUNCTION(BlueprintNativeEvent)
    void PlayerAddLevel(int32 InLevel);

    UFUNCTION(BlueprintNativeEvent)
    void PlayerAddAttributePoint(int32 InAttributePoint);

    UFUNCTION(BlueprintNativeEvent)
    void PlayerAddSkillPoint(int32 InSkillPoint);

    UFUNCTION(BlueprintNativeEvent)
    int32 PlayerGetXp() const;

    UFUNCTION(BlueprintNativeEvent)
    int32 PlayerGetLevelByXp(int32 InXp) const;

    UFUNCTION(BlueprintNativeEvent)
    int32 PlayerGetAttributePointReward(int32 InLevel) const;

    UFUNCTION(BlueprintNativeEvent)
    int32 PlayerGetSkillPointReward(int32 InLevel) const;

    UFUNCTION(BlueprintNativeEvent)
    void PlayerOnLevelUp();
};
