// Fill out your copyright notice in the Description page of Project Settings.


#include "VFX/Buff/DeBuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/Interface/CombatInterface.h"


UDeBuffNiagaraComponent::UDeBuffNiagaraComponent()
{
    bAutoActivate = false;
}

void UDeBuffNiagaraComponent::BeginPlay()
{
    Super::BeginPlay();

    TScriptInterface<ICombatInterface> CombatInterface(GetOwner());
    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
    {
        for (const auto& [DebuffTag, NiagaraEffect] : DebuffTagToEffect)
        {
            ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved)
               .AddUObject(this, &UDeBuffNiagaraComponent::OnDebuffTagChanged);
        }
    }
    else if (CombatInterface)
    {
        // 如果此时ASC未初始化, 在初始化ASC的回调中绑定OnDebuffTagChanged
        // WeakLambda会先检查InUserObject是否存在, 再执行Lambda函数
        CombatInterface->GetOnASCInitializedDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
        {
            for (const auto& [DebuffTag, NiagaraEffect] : DebuffTagToEffect)
            {
                InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved)
                     .AddUObject(this, &UDeBuffNiagaraComponent::OnDebuffTagChanged);
            }
        });
    }

    // 角色死亡时需要停止该特效组件
    if (CombatInterface)
    {
        CombatInterface->GetOnCharacterDiedDelegate().AddDynamic(this, &UDeBuffNiagaraComponent::OnCharacterDeath);
    }
}

void UDeBuffNiagaraComponent::OnDebuffTagChanged(const FGameplayTag DebuffTag, int32 NewCount)
{
    if (NewCount > 0)
    {
        CurDebuffTag = DebuffTag;
        SetupAssetByCurDebuffTag();
        Activate();
    }
    else
    {
        Deactivate();
    }
}

void UDeBuffNiagaraComponent::OnCharacterDeath(AActor* DeathActor)
{
    Deactivate();
}

void UDeBuffNiagaraComponent::SetupAssetByCurDebuffTag()
{
    if (DebuffTagToEffect.Contains(CurDebuffTag))
    {
        SetAsset(DebuffTagToEffect[CurDebuffTag]);
    }
}
