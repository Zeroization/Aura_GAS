// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interface/Interactable.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	AAuraEnemy();

	virtual void BeginPlay() override;
	virtual void InitAbilitySystem() override;

	/// >>>> Begin: Interactable Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/// >>>> End: Interactable Interface

	/// >>>> Begin: Combat Interface
	virtual int32 GetActorLevel() override;
	/// >>>> End: Combat Interface

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Defaults")
	int32 Level = 1;

	/* Begin: UI */
	/// 血条
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	/* End: UI */
};
