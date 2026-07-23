// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"

#include "Aura/Aura.h"


FAuraAbilityData UAuraAbilityInfo::FindAbilityDataByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
    auto* RetVal = AbilityInformation.FindByPredicate([&AbilityTag](const FAuraAbilityData& Data) -> bool
    {
        return Data.AbilityTag == AbilityTag;
    });

    if (RetVal == nullptr)
    {
        if (bLogNotFound)
        {
            UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Can't find data for AbilityTag [ %s ] on AuraAbilityInfo [ %s ]"),
                   __FUNCTION__,
                   *AbilityTag.ToString(),
                   *GetNameSafe(this));
        }
        return {};
    }
    return *RetVal;
}
