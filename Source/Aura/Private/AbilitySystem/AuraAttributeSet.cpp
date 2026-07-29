// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Game/AuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "Interaction/Interface/CombatInterface.h"
#include "Interaction/Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"
#include "UI/Widget/DamageFloatingTextComponent.h"


UAuraAttributeSet::UAuraAttributeSet()
{
    /// Begin Category: Vital Attributes >>>>>>>>>>
    InitHealth(50.f);
    InitMana(25.f);
    /// End Category: Vital Attributes   >>>>>>>>>>
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // _NOTIFY 会触发对应变量的OnRep_函数
    /// Begin Category: Vital Attributes >>>>>>>>>>
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    /// End Category: Vital Attributes   >>>>>>>>>>

    /// Begin Category: Primary Attributes >>>>>>>>>
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
    /// End Category: Primary Attributes >>>>>>>>>>>

    /// Begin Category: Secondary Attributes >>>>>>>
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitBonusDamage, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always)
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
    /// End Category: Secondary Attributes >>>>>>>>>
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // 在Attribute的Current Value改变前进行Clamp操作
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
    }
    else if (Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
    }
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // 获取Source和Target的相关信息
    FEffectProperties EffectProperties;
    SetEffectProperties(Data, EffectProperties);

    // 处理Health和Mana
    // 在GE之后对Current Value再次Clamp, 防止在PreAttributeChange()中Clamp错误的NewValue
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }

    // 处理IncomingDamage
    if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
    {
        const float LocalIncomingDamage = GetIncomingDamage();
        SetIncomingDamage(0.f);

        HandleIncomingDamage(EffectProperties, LocalIncomingDamage);
    }

    // 处理IncomingXp
    if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
    {
        const float LocalIncomingXp = GetIncomingXp();
        SetIncomingXp(0.f);

        HandleIncomingXp(EffectProperties, LocalIncomingXp);
    }
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldData);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldData);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldData);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldData);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldData);
}

void UAuraAttributeSet::OnRep_CriticalHitBonusDamage(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitBonusDamage, OldData);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldData);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldData);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldData);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldData);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldData);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldData) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldData);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
    Props.EffectContextHandle = Data.EffectSpec.GetContext();

    // Source: 引发GE的Actor
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
    if (IsValid(Props.SourceASC) &&
        Props.SourceASC->AbilityActorInfo.IsValid() &&
        Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
        Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
        if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
        {
            if (const APawn* SourcePawn = Cast<APawn>(Props.SourceAvatarActor))
            {
                Props.SourceController = SourcePawn->GetController();
            }
        }
        Props.SourceCharacter = Props.SourceController->GetCharacter();
    }

    // Target: 被GE施加效果的Actor
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.GetAvatarActor();
        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        if (Props.TargetController == nullptr && Props.TargetAvatarActor != nullptr)
        {
            if (const APawn* TargetPawn = Cast<APawn>(Props.TargetAvatarActor))
            {
                Props.TargetController = TargetPawn->GetController();
            }
        }
        Props.TargetCharacter = Props.TargetController->GetCharacter();
        Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
    }
}

void UAuraAttributeSet::ShowDamageText(const FEffectProperties& Props, float LocalIncomingDamage)
{
    const bool bBlockedHit = UAuraAbilitySystemLibrary::GetIsBlockedHit(Props.EffectContextHandle);
    const bool bCriticalHit = UAuraAbilitySystemLibrary::GetIsCriticalHit(Props.EffectContextHandle);

    FDamageFloatingTextProperty DamageFloatingTextProperty;
    DamageFloatingTextProperty.Damage = LocalIncomingDamage;
    if (bBlockedHit)
    {
        DamageFloatingTextProperty.DamageTypeFlags |= static_cast<uint8>(EAuraDamageType::Blocked);
    }
    if (bCriticalHit)
    {
        DamageFloatingTextProperty.DamageTypeFlags |= static_cast<uint8>(EAuraDamageType::Critical);
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(It->Get()))
        {
            PC->ShowDamageFloatingText(Props.TargetCharacter, DamageFloatingTextProperty);
        }
    }
}

void UAuraAttributeSet::SendXpRewardEvent(const FEffectProperties& Props)
{
    if (Props.TargetCharacter->Implements<UCombatInterface>())
    {
        const int32 TargetLevel = ICombatInterface::Execute_GetActorLevel(Props.TargetCharacter);
        const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClassEnum(Props.TargetCharacter);
        int32 XpReward = UAuraAbilitySystemLibrary::GetEnemyXpRewardByClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

        FGameplayEventData Payload;
        Payload.EventTag = AuraGameplayTags::Attribute::Meta::IncomingXp;
        Payload.EventMagnitude = XpReward;
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, AuraGameplayTags::Attribute::Meta::IncomingXp,
                                                                 Payload);
    }
}

void UAuraAttributeSet::HandleIncomingXp(const FEffectProperties& Props, float LocalIncomingXp)
{
    // GA_ListenForEvent施加GE_EventBasedEffect, 因此要对拥有者SourceCharacter增加经验
    ACharacter* SourceCharacter = Props.SourceCharacter;
    if (!SourceCharacter || !SourceCharacter->Implements<UCombatInterface>() || !SourceCharacter->Implements<UCombatInterface>())
    {
        return;
    }

    // 处理升级逻辑
    const int32 XpReward = FMath::RoundToInt(LocalIncomingXp);
    const int32 OldLevel = ICombatInterface::Execute_GetActorLevel(SourceCharacter);
    const int32 OldXp = IPlayerInterface::Execute_PlayerGetXp(SourceCharacter);

    const int32 NewXp = OldXp + XpReward;
    const int32 NewLevel = IPlayerInterface::Execute_PlayerGetLevelByXp(SourceCharacter, NewXp);
    const int32 DeltaLevel = NewLevel - OldLevel;

    IPlayerInterface::Execute_PlayerAddXp(Props.SourceCharacter, XpReward);

    if (DeltaLevel > 0)
    {
        int32 AttributePointReward = 0;
        int32 SkillPointReward = 0;
        for (int32 Level = OldLevel + 1; Level <= NewLevel; ++Level)
        {
            AttributePointReward += IPlayerInterface::Execute_PlayerGetAttributePointReward(SourceCharacter, Level);
            SkillPointReward += IPlayerInterface::Execute_PlayerGetSkillPointReward(SourceCharacter, Level);
        }
        IPlayerInterface::Execute_PlayerAddAttributePoint(SourceCharacter, AttributePointReward);
        IPlayerInterface::Execute_PlayerAddSkillPoint(SourceCharacter, SkillPointReward);
        IPlayerInterface::Execute_PlayerAddLevel(SourceCharacter, DeltaLevel);

        SetHealth(GetMaxHealth());
        SetMana(GetMaxMana());

        IPlayerInterface::Execute_PlayerOnLevelUp(Props.SourceCharacter);
    }
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props, float LocalIncomingDamage)
{
    if (LocalIncomingDamage > 0.f)
    {
        const float NewHealth = GetHealth() - LocalIncomingDamage;
        SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

        // 处理目标受伤逻辑
        const bool bIsDead = NewHealth <= 0.f;
        if (bIsDead)
        {
            AActor* TargetAvatarActor = Props.TargetAvatarActor;
            if (TargetAvatarActor->Implements<UCombatInterface>())
            {
                SendXpRewardEvent(Props);

                const TScriptInterface<ICombatInterface> CombatInterface = TScriptInterface<ICombatInterface>(TargetAvatarActor);
                CombatInterface->Die();
            }
        }
        else
        {
            FGameplayTagContainer TagContainer;
            TagContainer.AddTag(AuraGameplayTags::GE::HitReact.GetTag());
            Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
        }

        // 在敌人身上显示伤害数字
        if (Props.SourceCharacter != Props.TargetCharacter)
        {
            ShowDamageText(Props, LocalIncomingDamage);
        }
    }
}
