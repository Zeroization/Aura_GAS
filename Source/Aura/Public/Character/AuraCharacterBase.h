// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/Interface/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UMotionWarpingComponent;
class UAuraGameplayAbility;
class UGameplayEffect;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    AAuraCharacterBase();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    FORCEINLINE UAuraAttributeSet* GetAttributeSet() const { return AuraAttributeSet; }

protected:
    /// Begin Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    /// TObjectPtr<>是UE5新引入的指针类型, 用于替代传统UObject*, 更安全高效
    /// 有两个关键特性:
    ///		1. 访问追踪 Access Tracking:	记录和检测TObjectPtr<>的访问情况,
    ///									利于编辑器追踪UObject*, 对其进行
    ///									额外检查.
    ///		2. 延迟加载 Lazy Loading:	当对象来自外部包时，它可以保存一个轻量级
    ///								的句柄，在访问时才真正加载对象。
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
    TMap<FGameplayTag, FName> SocketTagToName;

    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
    TArray<FAuraTaggedMontage> AttackMontages;

    bool bIsDead = false;

    /* 随从 */
    int32 MinionCount = 0;
    /* 随从 */

    /// 所有客户端: 该角色死亡的逻辑(例如播放动画等)
    UFUNCTION(NetMulticast, Reliable)
    virtual void MulticastOnCharacterDeath();
    /// 服务器: 该角色死亡的逻辑
    virtual void Die() override;
    virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
    virtual FAuraTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
    virtual TArray<FAuraTaggedMontage> GetTaggedAttackMontages_Implementation() override;
    virtual UAnimMontage* GetHitReactMontage_Implementation() override;
    virtual bool IsDead_Implementation() const override;
    virtual AActor* GetAvatar_Implementation() override;
    virtual int32 GetMinionCount_Implementation() override;
    virtual void IncreaseMinionCountByAmount_Implementation(int32 Amount) override;
    /// End Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    /// Begin: Ability System <<<<<<<<<<<<<<<
    UPROPERTY()
    TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
    TSubclassOf<UGameplayEffect> InitPrimaryAttrGEClass;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
    TSubclassOf<UGameplayEffect> InitSecondaryAttrGEClass;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AuraCharacter|Attributes")
    TSubclassOf<UGameplayEffect> InitVitalAttrGEClass;

    virtual void InitAbilitySystem();
    virtual void InitDefaultAttributes() const;

    void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GEClass, float Level = 1.f) const;
    void GrantCharacterStartupAbilities();
    /// End: Ability System   <<<<<<<<<<<<<<<

    /// Begin: Animation <<<<<<<<<<<<<<<<<<<<
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AuraCharacter|Animation")
    TObjectPtr<UMotionWarpingComponent> MotionWarping;

    UFUNCTION(BlueprintCallable)
    bool CheckMotionWarpTargetExists(const FName& WarpTargetName);
    /// End:   Animation <<<<<<<<<<<<<<<<<<<<

    /* Begin: SFX */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
    TObjectPtr<UMaterialInstance> CharacterDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
    TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
    TObjectPtr<UNiagaraSystem> BloodImpactEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraCharacter|SFX")
    TObjectPtr<USoundBase> DeathSound;

    virtual UNiagaraSystem* GetImpactBloodEffect_Implementation() override;

    /// 在角色死亡时替换溶解材质
    void Dissolve();

    /// 在蓝图中实现角色慢慢溶解的效果
    UFUNCTION(BlueprintImplementableEvent)
    void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMIs);
    /* End: SFX */

    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Abilities")
    TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;

    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Abilities")
    TArray<TSubclassOf<UAuraGameplayAbility>> StartupPassiveAbilities;

    UPROPERTY(EditAnywhere, Category = "AuraCharacter|Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
};
