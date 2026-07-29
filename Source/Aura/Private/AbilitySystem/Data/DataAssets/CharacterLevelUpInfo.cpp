// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/DataAssets/CharacterLevelUpInfo.h"

#include "Aura/Aura.h"

int32 UCharacterLevelUpInfo::GetLevelByXP(int32 XP, bool bOutputLog) const
{
    const int32 LevelIdx = LevelUpInfos.IndexOfByPredicate([XP, bOutputLog](const FAuraSingleLevelUpInfo& Info) -> bool
    {
        if (bOutputLog)
        {
            UE_LOG(LogAuraGame, Warning, TEXT("[%hs]: Cur XP %d, LvlUpReq %d"), __FUNCTION__, XP, Info.LevelUpRequirement);
        }
        return XP <= Info.LevelUpRequirement;
    });
    return LevelIdx;
}
