// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraAbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbilityData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Ability的GameplayTag"))
    FGameplayTag AbilityTag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="Ability的唯一冷却Tag"))
    FGameplayTag CooldownTag;

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag InputTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="对应SpellGlobe上的技能图标"))
    TObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="对应SpellGlobe上的技能背景"))
    TObjectPtr<UMaterialInterface> SkillBackground;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilityInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAuraAbilityData> AbilityInformation;

    FAuraAbilityData FindAbilityDataByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = true) const;
};
