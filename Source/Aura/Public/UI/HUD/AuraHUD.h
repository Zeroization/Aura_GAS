// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraSpellMenuWidgetController;
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

    UFUNCTION(BlueprintPure)
    UAuraSpellMenuWidgetController* GetAuraSpellMenuWidgetController(const FAuraWidgetControllerParams& InParams);

protected:
    virtual void BeginPlay() override;

private:
    /// Begin Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>
    UPROPERTY(EditAnywhere, Category = "Aura HUD|Overlay")
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Aura HUD|Overlay")
    TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;

    UPROPERTY()
    TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
    /// End Category: Widget Overlay>>>>>>>>>>>>>>>>>>>>>>>>>

    /// Begin Category: Widget Attribute Menu >>>>>>>>>>>>>>>
    UPROPERTY(EditAnywhere, Category = "Aura HUD|Attribute Menu")
    TSubclassOf<UAuraAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UAuraAttributeMenuWidgetController> AttributeMenuWidgetController;
    /// End Category: Widget Attribute Menu >>>>>>>>>>>>>>>>>

    /// Begin Category: Widget Spell Menu >>>>>>>>>>>>>>>
    UPROPERTY(EditAnywhere, Category = "Aura HUD|Spell Menu")
    TSubclassOf<UAuraSpellMenuWidgetController> SpellMenuWidgetControllerClass;

    UPROPERTY()
    TObjectPtr<UAuraSpellMenuWidgetController> SpellMenuWidgetController;
    /// End Category: Widget Spell Menu >>>>>>>>>>>>>>>>>
};
