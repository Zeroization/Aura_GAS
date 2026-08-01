// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UCharacterLevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeDelegate, int32);

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCharacterLevelUpInfo> AuraLevelUpInfo;

    FOnPlayerStatChangeDelegate OnPlayerXpChange;
    FOnPlayerStatChangeDelegate OnPlayerLevelChange;
    FOnPlayerStatChangeDelegate OnPlayerAttributePointChange;
    FOnPlayerStatChangeDelegate OnPlayerSkillPointChange;

    AAuraPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }
    FORCEINLINE int32 GetPlayerLevel() const { return Level; }
    FORCEINLINE int32 GetXp() const { return Xp; }
    FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
    FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }

    void AddLevel(int32 InLevel);
    void AddXp(int32 InXp);
    void AddAttributePoint(int32 InAttributePoint);
    void AddSkillPoint(int32 InSkillPoint);

    void SetLevel(int32 NewLevel);
    void SetXp(int32 NewXp);
    void SetAttributePoint(int32 NewAttributePoint);
    void SetSkillPoint(int32 NewSkillPoint);

protected:
    /// Begin: Ability System <<<<<<<<<<<<<<<
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
    /// End: Ability System   <<<<<<<<<<<<<<<

private:
    UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Level)
    int32 Level = 1;

    UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Xp)
    int32 Xp = 0;

    UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_AttributePoint)
    int32 AttributePoint = 0;

    UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_SkillPoint)
    int32 SkillPoint = 0;

    UFUNCTION()
    void OnRep_Level(int32 OldLevel);

    UFUNCTION()
    void OnRep_Xp(int32 OldXp);

    UFUNCTION()
    void OnRep_AttributePoint(int32 OldAttributePoint);

    UFUNCTION()
    void OnRep_SkillPoint(int32 OldSkillPoint);
};
