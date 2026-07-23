// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"


UAuraOverlayWidgetController* AAuraHUD::GetAuraOverlayWidgetController(const FAuraWidgetControllerParams& InParams)
{
    if (OverlayWidgetController == nullptr)
    {
        OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
        OverlayWidgetController->SetWidgetControllerParams(InParams);
        OverlayWidgetController->BindDelegateCallbackFunctions();
    }
    // else
    // {
    // 	OverlayWidgetController->SetWidgetControllerParams(InParams);
    // }

    return OverlayWidgetController;
}

void AAuraHUD::InitHUD(AAuraPlayerController* InPlayerController, AAuraPlayerState* InPlayerState,
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
    UAuraOverlayWidgetController* WidgetController = GetAuraOverlayWidgetController(InParams);
    OverlayWidget->SetWidgetController(WidgetController);

    ///
    /// 注: 这里和教程不一样, 3和4对调了, 原因是我在初始化技能图标时服务器没有，客户端有，问AI好像是UI生命周期的问题,
    ///     Slate Widget未完全建立时进行数据的多播, 导致无效多播.
    /// UE版本5.4.4
    
    // 3. 添加到角色视口中
    OverlayWidget->AddToViewport();
    
    // 4. 通过WidgetController初始化UI数据
    WidgetController->BroadcastInitialValues();
    WidgetController->BroadcastAbilityData();
}

UAuraAttributeMenuWidgetController* AAuraHUD::GetAuraAttributeMenuWidgetController(const FAuraWidgetControllerParams& InParams)
{
    if (AttributeMenuWidgetController == nullptr)
    {
        AttributeMenuWidgetController = NewObject<UAuraAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
        AttributeMenuWidgetController->SetWidgetControllerParams(InParams);
        AttributeMenuWidgetController->BindDelegateCallbackFunctions();
    }
    // else
    // {
    // 	AttributeMenuWidgetController->SetWidgetControllerParams(InParams);
    // }

    return AttributeMenuWidgetController;
}

void AAuraHUD::BeginPlay()
{
    Super::BeginPlay();
}
