// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interactable.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()

public:
	AAuraEnemy();
 
	/// >>>> Begin: Interface IInteractable
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/// >>>> End: Interface IInteractable
};
