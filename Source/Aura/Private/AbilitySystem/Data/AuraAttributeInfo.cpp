// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraAttributeInfo.h"


FAuraAttributeData UAuraAttributeInfo::FindAttributeDataByTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	auto* RetVal = AttributeDataList.FindByPredicate([&AttributeTag](const FAuraAttributeData& Data) -> bool
	{
		return Data.AttributeTag == AttributeTag;
	});

	if (RetVal == nullptr)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogTemp, Error, TEXT("[%hs]: Can't find data for AttributeTag [ %s ] on AuraAttributeInfo [ %s ]"),
			       __FUNCTION__,
			       *AttributeTag.ToString(),
			       *GetNameSafe(this));
		}
		return {};
	}
	return *RetVal;
}
