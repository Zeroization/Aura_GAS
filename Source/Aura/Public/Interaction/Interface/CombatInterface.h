// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "CombatInterface.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FAuraTaggedMontageAttachment
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<USoundBase> ImpactSound;
};

// 将蒙太奇和GameplayTag: MontageToSocket.* 作映射的结构体 
USTRUCT(BlueprintType)
struct FAuraTaggedMontage
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MontageTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FAuraTaggedMontageAttachment Attachment;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
#pragma region 通用
    virtual int32 GetActorLevel();
    virtual void Die() = 0;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FAuraTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TArray<FAuraTaggedMontage> GetTaggedAttackMontages();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    AActor* GetAvatar();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    int32 GetMinionCount();
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void IncreaseMinionCountByAmount(int32 Amount);
    
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    UNiagaraSystem* GetImpactBloodEffect();

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void SetFacingWarpTarget(const FVector& TargetLocation);
#pragma endregion

#pragma region 敌人
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    UAnimMontage* GetHitReactMontage();

    // 设置敌人的CombatTarget
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetCombatTarget(AActor* InCombatTarget);

    // 获取敌人的CombatTarget
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    AActor* GetCombatTarget() const;
#pragma endregion
};
