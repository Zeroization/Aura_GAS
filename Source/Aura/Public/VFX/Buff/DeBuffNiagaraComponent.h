// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DeBuffNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UDeBuffNiagaraComponent : public UNiagaraComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TMap<FGameplayTag, TObjectPtr<UNiagaraSystem>> DebuffTagToEffect;

    UDeBuffNiagaraComponent();

    virtual void BeginPlay() override;

    void OnDebuffTagChanged(const FGameplayTag DebuffTag, int32 NewCount);
    UFUNCTION()
    void OnCharacterDeath(AActor* DeathActor);

private:
    FGameplayTag CurDebuffTag;

    void SetupAssetByCurDebuffTag();
};
