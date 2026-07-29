// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/DataAssets/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/Interface/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
    GENERATED_BODY()

public:
    /// Begin Category: VFX <<<<<<<<<<<<<<
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AuraCharacter|VFX")
    TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
    /// End Category: VFX <<<<<<<<<<<<<<<<

    AAuraCharacter();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    /// Begin: Combat Interface <<<<
    virtual int32 GetActorLevel_Implementation() override;
    virtual ECharacterClass GetCharacterClassEnum_Implementation() override;
    /// End: Combat Interface <<<<<<

    /// Begin: Player Interface <<<<
    virtual void PlayerAddXp_Implementation(int32 InXp) override;
    virtual void PlayerAddLevel_Implementation(int32 InLevel) override;
    virtual void PlayerAddAttributePoint_Implementation(int32 InAttributePoint) override;
    virtual void PlayerAddSkillPoint_Implementation(int32 InSkillPoint) override;
    virtual int32 PlayerGetXp_Implementation() const override;
    virtual int32 PlayerGetLevelByXp_Implementation(int32 InXp) const override;
    virtual int32 PlayerGetAttributePointReward_Implementation(int32 InLevel) const override;
    virtual int32 PlayerGetSkillPointReward_Implementation(int32 InLevel) const override;
    virtual void PlayerOnLevelUp_Implementation() override;
    /// End: Player Interface <<<<<<

private:
    /// Begin Category: Camera <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Camera")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Camera")
    TObjectPtr<UCameraComponent> CameraComponent;
    /// End Category: Camera <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    /// Begin: Init <<<<<<<<<<<<<<<<<<<<<<<<<<<<
    virtual void InitAbilitySystem() override;
    void InitPlayerHUD();
    /// End: Init  <<<<<<<<<<<<<<<<<<<<<<<<<<<<

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayLevelUpVFX() const;
};
