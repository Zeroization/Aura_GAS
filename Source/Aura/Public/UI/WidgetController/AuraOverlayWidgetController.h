// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FDisplayWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayMessage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> DisplayImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> DisplayWidget;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FDisplayWidgetRow, Row);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature OnDisplayMessageRowReceived;

	virtual void BroadcastInitialValues() override;
	virtual void BindDelegateCallbackFunctions() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/**
	 * 根据 GameplayTag 进行查表的工具函数, 行名必须是 GameplayTag 名, TODO: 创建一个工具函数库
	 * @tparam DTRowType 任意数据表格的行类型
	 * @param DataTable 数据表格指针
	 * @param Tag Gameplay Tag
	 * @return 指向表行数据的指针
	 */
	template <typename DTRowType>
	static DTRowType* GetDataTableRowByGameplayTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnManaChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const;
};

template <typename DTRowType>
DTRowType* UAuraOverlayWidgetController::GetDataTableRowByGameplayTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<DTRowType>(Tag.GetTagName(), TEXT(""));
}
