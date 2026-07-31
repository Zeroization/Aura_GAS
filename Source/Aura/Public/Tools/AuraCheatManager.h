// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "AuraCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraCheatManager : public UCheatManager
{
    GENERATED_BODY()

public:
    UFUNCTION(Exec)
    void AddXp(int32 Amount);

    UFUNCTION(Exec)
    void AddAttributePoint(int32 Amount);

    UFUNCTION(Exec)
    void AddSkillPoint(int32 Amount);
};
