// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * MVC UI 架构
 * View, 通过Controller获取数据, 用自身展示
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
};
