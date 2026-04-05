// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interface/Interactable.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
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
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float DefaultWalkSpeed = 250.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bDoHitReacting = false;

	AAuraEnemy();

	void OnGEHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void BeginPlay() override;
	virtual void InitAbilitySystem() override;
	virtual void InitDefaultAttributes() const override;

	/// >>>> Begin: Interactable Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/// >>>> End: Interactable Interface

	/// >>>> Begin: Combat Interface
	virtual int32 GetActorLevel() override;
	/// >>>> End: Combat Interface

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::ECC_Warrior;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	/* Begin: UI */
	/// 血条
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	/* End: UI */
};
