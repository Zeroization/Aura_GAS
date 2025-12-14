// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interface/Interactable.h"
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

	virtual void BeginPlay() override;
	virtual void InitAbilitySystem() override;

	/// >>>> Begin: Interface IInteractable
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/// >>>> End: Interface IInteractable

	/// >>>> Begin: Combat Interface
	virtual int32 GetActorLevel() override;
	/// >>>> End: Combat Interface

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
	int32 Level = 1;
};
