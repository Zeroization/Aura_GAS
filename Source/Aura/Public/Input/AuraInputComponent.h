// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"


template <class UserClass>
struct FInputActionCallbacks
{
	using CallbackFuncType = TMemFunPtrType<false, UserClass, void(FGameplayTag)>::Type;

	CallbackFuncType OnPressed = nullptr;
	CallbackFuncType OnReleased = nullptr;
	CallbackFuncType OnHeld = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass>
	void BindAbilityInputAction(const UAuraInputConfig* InputConfig, UserClass* ToBindObject,
	                            const FInputActionCallbacks<UserClass>& Callbacks);
};

template <class UserClass>
void UAuraInputComponent::BindAbilityInputAction(const UAuraInputConfig* InputConfig, UserClass* ToBindObject,
                                                 const FInputActionCallbacks<UserClass>& Callbacks)
{
	checkf(IsValid(InputConfig), TEXT("[%hs]: InputConfig is nullptr, plz check!"), __FUNCTION__);

	for (const FAuraInputAction& IA : InputConfig->AbilityInputActions)
	{
		if (!IsValid(IA.InputAction) || !IA.Tag.IsValid())
		{
			continue;
		}

		if (Callbacks.OnPressed)
		{
			BindAction(IA.InputAction, ETriggerEvent::Started, ToBindObject, Callbacks.OnPressed, IA.Tag);
		}

		if (Callbacks.OnReleased)
		{
			BindAction(IA.InputAction, ETriggerEvent::Completed, ToBindObject, Callbacks.OnReleased, IA.Tag);
		}

		if (Callbacks.OnHeld)
		{
			BindAction(IA.InputAction, ETriggerEvent::Triggered, ToBindObject, Callbacks.OnHeld, IA.Tag);
		}
	}
}
