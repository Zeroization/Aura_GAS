// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Game/AuraGameplayTags.h"
#include "AuraAbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="该GA所属的GameplayTag"))
    FGameplayTag AbilityTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="该GA的唯一冷却Tag"))
    FGameplayTag CooldownTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="该GA的技能类型Tag"))
    FGameplayTag SkillTypeTag = AuraGameplayTags::GA::SkillType::None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="解锁该GA的玩家等级"))
    int32 LevelRequirement = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="该GA的UGameplayAbility类"))
    TSubclassOf<UGameplayAbility> Ability;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="对应SpellGlobe上的技能图标"))
    TObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip="对应SpellGlobe上的技能背景"))
    TObjectPtr<UMaterialInterface> SkillBackground;

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag InputTag;

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag SkillStatusTag = AuraGameplayTags::GA::SkillStatus::Locked;
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

    UFUNCTION(BlueprintCallable)
    bool FindAbilityDataByTag(FAuraAbilityData& OutAbilityData, const FGameplayTag& AbilityTag, bool bLogNotFound = true);
    
    FAuraAbilityData* ModifyAbilityDataByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = true);
};
