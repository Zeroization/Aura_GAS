// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

/// 职业枚举
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    ECC_Elementalist UMETA(DisplayName = "Elementalist"), // 法师
    ECC_Warrior UMETA(DisplayName = "Warrior"),           // 战士
    ECC_Ranger UMETA(DisplayName = "Ranger"),             // 射手
    MAX UMETA(Hidden = true)
};

/// 职业默认信息结构体
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TSubclassOf<UGameplayEffect> PrimaryAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
};

/**
 * 职业信息DataAsset
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
    TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfoMap;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults|Common|GAS")
    TSubclassOf<UGameplayEffect> SecondaryAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults|Common|GAS")
    TSubclassOf<UGameplayEffect> VitalAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults|Common|GAS")
    TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults|Common|Damage")
    TObjectPtr<UCurveTable> DamageCalcCoefficientTable;

    FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass ClassEnum);
};
