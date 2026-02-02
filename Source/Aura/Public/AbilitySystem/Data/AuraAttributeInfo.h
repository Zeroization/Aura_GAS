// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAttributeInfo.generated.h"

struct FGameplayAttribute;
/**
* \brief 单个 GAS Primary/Secondary Attribute 的相关信息
*/
USTRUCT(BlueprintType)
struct FAuraAttributeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Attribute 的 GameplayTag"))
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Attribute 展示给 UI 的名字"))
	FText AttributeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Attribute 展示给 UI 的相关描述"))
	FText AttributeDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Attribute 本体, 用于和该Data作映射关系"))
	FGameplayAttribute Attribute;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * \brief GAS Primary/Secondary Attribute 相关的 DataAsset, 用于将其打包发送给 UI 进行对应 Attribute 的视觉呈现
 */
UCLASS()
class AURA_API UAuraAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeData> AttributeDataList;

	FAuraAttributeData FindAttributeDataByTag(const FGameplayTag& AttributeTag, bool bLogNotFound = true) const;
};
