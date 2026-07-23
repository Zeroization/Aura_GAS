// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

#include "InputAction.h"
#include "Aura/Aura.h"


TObjectPtr<UInputAction> UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraInputAction& IA : AbilityInputActions)
	{
		if (IsValid(IA.InputAction) && IA.Tag == Tag)
		{
			return IA.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Can't find AbilityInputAction by tag [ %s ], in [ %s ]"), __FUNCTION__, *Tag.ToString(),
		       *GetNameSafe(this));
	}

	return nullptr;
}
