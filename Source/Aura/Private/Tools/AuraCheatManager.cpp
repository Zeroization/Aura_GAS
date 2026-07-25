// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/AuraCheatManager.h"

#include "Aura/Aura.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraCheatManager::AddXp(int32 Amount)
{
    AAuraPlayerController* AuraPC = CastChecked<AAuraPlayerController>(GetOuterAPlayerController());

    UE_LOG(LogAuraGame, Warning, TEXT("[%hs]: Add [%d] xp to player %s"), __FUNCTION__, Amount, *AuraPC->PlayerState->GetPlayerName());
    AuraPC->ServerAddXp(Amount);
}
