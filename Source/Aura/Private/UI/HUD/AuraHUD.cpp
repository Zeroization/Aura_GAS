// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"


UAuraOverlayWidgetController* AAuraHUD::GetAuraOverlayWidgetController()
{
	checkf(OverlayWidgetController != nullptr, TEXT("[%hs] AuraOverlayWidgetController is nullptr !!!"), __FUNCTION__);
	return OverlayWidgetController;
}

UAuraOverlayWidgetController* AAuraHUD::SetAuraOverlayWidgetController(const FAuraWidgetControllerParams& InParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(InParams);
		OverlayWidgetController->BindDelegateCallbackFunctions();
	}
	else
	{
		OverlayWidgetController->SetWidgetControllerParams(InParams);
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlayWidget(AAuraPlayerController* InPlayerController, AAuraPlayerState* InPlayerState,
                                 UAuraAbilitySystemComponent* InAbilitySystemComponent,
                                 UAuraAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass,
	       TEXT("Overlay widget controller class uninitialized, please fill out BP_AuraHUD"));

	// 1. 创建Widget
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);

	// 2. 初始化WidgetController, 并和Widget绑定
	const FAuraWidgetControllerParams InParams(InPlayerController, InPlayerState, InAbilitySystemComponent,
	                                           InAttributeSet);
	UAuraOverlayWidgetController* WidgetController = SetAuraOverlayWidgetController(InParams);
	OverlayWidget->SetWidgetController(WidgetController);

	// 3. 通过WidgetController初始化UI数据
	WidgetController->BroadcastInitialValues();

	// 4. 添加到角色视口中
	OverlayWidget->AddToViewport();
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
}
