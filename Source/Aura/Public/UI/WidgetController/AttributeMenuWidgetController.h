// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAuraAttributeInfo;
struct FAuraAttributeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeDataSignature, const FAuraAttributeData&, Info);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraAttributeMenuWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FAttributeDataSignature AttributeDataDelegate;

    UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
    FOnPlayerStatChangedSignature PlayerAttributePointChangeDelegate;

    virtual void BindDelegateCallbackFunctions() override;
    virtual void BroadcastInitialValues() override;

    UFUNCTION(BlueprintCallable)
    void UpgradePrimaryAttribute(const FGameplayTag& PrimaryAttributeTag, int32 CostAttributePoint = 1);

    /// 设置AttributeData中各属性的初值(只有Value无初值, 要向AttributeSet获取), 并将其通过委托广播给UI
    /// \param AttributeData 要广播的AttributeData
    void BroadcastAttributeData(FAuraAttributeData& AttributeData) const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAuraAttributeInfo> AttributeInfo;
};
