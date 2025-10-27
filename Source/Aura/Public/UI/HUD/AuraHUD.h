// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
struct FAuraWidgetControllerParams;
class UAuraOverlayWidgetController;
class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	/// Begin Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UFUNCTION(BlueprintPure)
	UAuraOverlayWidgetController* GetAuraOverlayWidgetController();

	UFUNCTION(BlueprintCallable)
	UAuraOverlayWidgetController* SetAuraOverlayWidgetController(const FAuraWidgetControllerParams& InParams);

	UFUNCTION(BlueprintCallable)
	void InitOverlayWidget(AAuraPlayerController* InPlayerController, AAuraPlayerState* InPlayerState,
	                       UAuraAbilitySystemComponent* InAbilitySystemComponent,
	                       UAuraAttributeSet* InAttributeSet);
	/// End Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>>>

protected:
	virtual void BeginPlay() override;

private:
	/// Begin Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY(EditAnywhere, Category = "Widget Overlay")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget Overlay")
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
	/// End Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>>>
};
