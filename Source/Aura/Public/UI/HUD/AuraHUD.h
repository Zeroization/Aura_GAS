// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraAttributeMenuWidgetController;
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
	UFUNCTION(BlueprintCallable)
	void InitHUD(AAuraPlayerController* InPlayerController, AAuraPlayerState* InPlayerState,
						   UAuraAbilitySystemComponent* InAbilitySystemComponent,
						   UAuraAttributeSet* InAttributeSet);

	UFUNCTION(BlueprintPure)
	UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const FAuraWidgetControllerParams& InParams);

	UFUNCTION(BlueprintPure)
	UAuraAttributeMenuWidgetController* GetAuraAttributeMenuWidgetController(const FAuraWidgetControllerParams& InParams);

protected:
	virtual void BeginPlay() override;

private:
	/// Begin Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>
	UPROPERTY(EditAnywhere, Category = "Widget|Overlay")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget|Overlay")
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
	/// End Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>>>

	/// Begin Category: Widget Attribute Menu >>>>>>>>>>>>>>>
	UPROPERTY(EditAnywhere, Category = "Widget|Attribute Menu")
	TSubclassOf<UAuraUserWidget> AttributeMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget|Attribute Menu")
	TSubclassOf<UAuraAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> AttributeMenuWidget;

	UPROPERTY()
	TObjectPtr<UAuraAttributeMenuWidgetController> AttributeMenuWidgetController;
	/// End Category: Widget Attribute Menu >>>>>>>>>>>>>>>>>
};
