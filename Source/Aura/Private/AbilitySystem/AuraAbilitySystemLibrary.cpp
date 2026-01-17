// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"


UAuraOverlayWidgetController* UAuraAbilitySystemLibrary::GetAuraOverlayWidgetController(const UObject* WorldContextObject)
{
	// 从本地玩家获取UI Controller
	if (AAuraPlayerController* PlayerController =
		Cast<AAuraPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
			UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());
			const FAuraWidgetControllerParams Params(PlayerController, PlayerState, ASC, AttributeSet);
			return AuraHUD->SetAuraOverlayWidgetController(Params);
		}
	}

	return nullptr;
}
