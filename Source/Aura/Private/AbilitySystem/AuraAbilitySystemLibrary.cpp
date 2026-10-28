// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/DataAssets/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Game/AuraGameplayTags.h"
#include "Game/AuraGameStateBase.h"
#include "Interaction/Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetEnemyCharacterClassInfo(const UObject* WorldContextObject)
{
    if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
    {
        return AuraGameMode->EnemyCharacterClassInfo;
    }

    return nullptr;
}

void UAuraAbilitySystemLibrary::InitEnemyDefaultAttributesByClass(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC,
                                                                  ECharacterClass EnemyClass, int32 EnemyLevel)
{
    if (UCharacterClassInfo* EnemyClassInfo = GetEnemyCharacterClassInfo(WorldContextObject))
    {
        FCharacterClassDefaultInfo EnemyClassDefaultInfo = EnemyClassInfo->GetClassDefaultInfo(EnemyClass);

        AActor* AvatarActor = ASC->GetAvatarActor();
        // Primary Attributes
        FGameplayEffectContextHandle PrimaryAttrGEContextHandle = ASC->MakeEffectContext();
        PrimaryAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle PrimaryAttrGESpecHandle = ASC->MakeOutgoingSpec(EnemyClassDefaultInfo.PrimaryAttributes, EnemyLevel,
                                                                                        PrimaryAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttrGESpecHandle.Data);

        // Secondary Attributes
        FGameplayEffectContextHandle SecondaryAttrGEContextHandle = ASC->MakeEffectContext();
        SecondaryAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle SecondaryAttrGESpecHandle = ASC->MakeOutgoingSpec(
            EnemyClassInfo->SecondaryAttributes, EnemyLevel, SecondaryAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttrGESpecHandle.Data);

        // Vital Attributes
        FGameplayEffectContextHandle VitalAttrGEContextHandle = ASC->MakeEffectContext();
        VitalAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle VitalAttrGESpecHandle = ASC->MakeOutgoingSpec(
            EnemyClassInfo->VitalAttributes, EnemyLevel, VitalAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*VitalAttrGESpecHandle.Data);
    }
}

void UAuraAbilitySystemLibrary::GrantEnemyStartupAbilities(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC,
                                                           ECharacterClass CharacterClass, int32 EnemyLevel)
{
    if (UCharacterClassInfo* EnemyClassInfo = GetEnemyCharacterClassInfo(WorldContextObject))
    {
        // 职业GA
        const FCharacterClassDefaultInfo ClassDefaultInfo = EnemyClassInfo->CharacterClassInfoMap[CharacterClass];
        if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
        {
            EnemyLevel = ICombatInterface::Execute_GetActorLevel(ASC->GetAvatarActor());
        }
        for (const auto& AbilityClass : ClassDefaultInfo.DefaultAbilities)
        {
            FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, EnemyLevel);
            ASC->GiveAbility(GASpec);
        }

        // 通用GA
        for (const auto& AbilityClass : EnemyClassInfo->CommonAbilities)
        {
            FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, 1);
            ASC->GiveAbility(GASpec);
        }
    }
}

int32 UAuraAbilitySystemLibrary::GetEnemyXpRewardByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass,
                                                                 int32 EnemyLevel)
{
    if (UCharacterClassInfo* EnemyClassInfo = GetEnemyCharacterClassInfo(WorldContextObject))
    {
        const FCharacterClassDefaultInfo ClassDefaultInfo = EnemyClassInfo->CharacterClassInfoMap[CharacterClass];
        return ClassDefaultInfo.XpReward.GetValueAtLevel(EnemyLevel);
    }
    return 0;
}

UAuraAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
    if (AAuraGameStateBase* AuraGameState = Cast<AAuraGameStateBase>(UGameplayStatics::GetGameState(WorldContextObject)))
    {
        return AuraGameState->AbilityInfo;
    }

    return nullptr;
}

bool UAuraAbilitySystemLibrary::GetIsBlockedHit(const FGameplayEffectContextHandle& GEContextHandle)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        return GEContext->GetIsBlockedHit();
    }
    return false;
}

bool UAuraAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& GEContextHandle)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        return GEContext->GetIsCriticalHit();
    }
    return false;
}

bool UAuraAbilitySystemLibrary::GetIsDoTDamage(const FGameplayEffectContextHandle& GEContextHandle)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        return GEContext->GetIsDoTDamage();
    }
    return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& GEContextHandle, bool InIsBlockedHit)
{
    if (FAuraGameplayEffectContext* GEContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        GEContext->SetIsBlockedHit(InIsBlockedHit);
    }
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GEContextHandle, bool InIsCriticalHit)
{
    if (FAuraGameplayEffectContext* GEContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        GEContext->SetIsCriticalHit(InIsCriticalHit);
    }
}

void UAuraAbilitySystemLibrary::SetIsDoTDamage(FGameplayEffectContextHandle& GEContextHandle, bool InIsDoTDamage)
{
    if (FAuraGameplayEffectContext* GEContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        GEContext->SetIsDoTDamage(InIsDoTDamage);
    }
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyAuraDamageGE(const FAuraDamageEffectParams& Params)
{
    AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();

    // 创建GEContext, 并添加自定义信息
    FGameplayEffectContextHandle ContextHandle = Params.SourceASC->MakeEffectContext();
    ContextHandle.AddSourceObject(SourceAvatarActor);
    if (FAuraGameplayEffectContext* GEContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
    {
        // Debuffs
        GEContext->ResetDebuffs();
        for (auto& DebuffParams : Params.Debuffs)
        {
            GEContext->AddDebuff(DebuffParams);
        }

        // DeathImpulse
        GEContext->SetDeathImpulse(Params.DeathImpulse);
    }
    else
    {
        UE_LOG(LogAuraGame, Error, TEXT("[%hs]: Can't convert to FAuraGameplayEffectContext."), __FUNCTION__);
        return ContextHandle;
    }

    // 将GEContext关联至新建的GESpec, 并绑定伤害相关信息
    FGameplayEffectSpecHandle SpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, ContextHandle);
    for (auto& [DamageElemTypeTag, BaseDamage] : Params.DamageElemTypeToBaseDamage)
    {
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageElemTypeTag, BaseDamage);
    }

    Params.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
    return ContextHandle;
}

bool UAuraAbilitySystemLibrary::TryGetDebuffByTag(const FGameplayEffectContextHandle& GEContextHandle, FAuraDebuffParams& OutDebuffParams,
                                                  const FGameplayTag& DebuffTag)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        return GEContext->TryGetDebuffParams(DebuffTag, OutDebuffParams);
    }

    return false;
}

bool UAuraAbilitySystemLibrary::GetDebuffs(const FGameplayEffectContextHandle& GEContextHandle, TArray<FAuraDebuffParams>& OutDebuffs)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        OutDebuffs = GEContext->GetDebuffs();
        return true;
    }
    return false;
}

bool UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& GEContextHandle, FVector& OutDeathImpulse)
{
    if (const FAuraGameplayEffectContext* GEContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
    {
        OutDeathImpulse = GEContext->GetDeathImpulse();
        return true;
    }

    OutDeathImpulse = FVector::ZeroVector;
    return false;
}

bool UAuraAbilitySystemLibrary::ContainsDamageTypeByProperty(const FDamageFloatingTextProperty& Property,
                                                             EAuraDamageType DamageType)
{
    return ContainsDamageTypeByFlags(Property.DamageTypeFlags, DamageType);
}

bool UAuraAbilitySystemLibrary::ContainsDamageTypeByFlags(uint8 DamageTypeFlags, EAuraDamageType DamageType)
{
    const uint8 DamageTypeFlag = static_cast<uint8>(DamageType);
    return DamageTypeFlag != static_cast<uint8>(EAuraDamageType::None) && (DamageTypeFlags & DamageTypeFlag) != 0;
}

bool UAuraAbilitySystemLibrary::IsFriendlyFire(AActor* First, AActor* Second)
{
    if (IsValid(First) && IsValid(Second))
    {
        return (First->ActorHasTag(AURA_ACTOR_FNAME_TAG_PLAYER) && Second->ActorHasTag(AURA_ACTOR_FNAME_TAG_PLAYER)) ||
            (First->ActorHasTag(AURA_ACTOR_FNAME_TAG_ENEMY) && Second->ActorHasTag(AURA_ACTOR_FNAME_TAG_ENEMY));
    }

    checkf(false, TEXT("Invalid actor"));
    return false;
}

void UAuraAbilitySystemLibrary::QueryActorsInSphere(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                                    const TArray<AActor*> ActorsToIgnore, float SphereRadius, const FVector& SphereOrigin)
{
    // 1.设置碰撞检测参数
    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorsToIgnore);

    // 2.进行碰撞检测
    // Ref: UGameplayStatics::ApplyRadialDamageWithFalloff()
    TArray<FOverlapResult> Overlaps;
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
                                        FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
                                        FCollisionShape::MakeSphere(SphereRadius), SphereParams);

        for (auto& Overlap : Overlaps)
        {
            if (AActor* OverlappedActor = Overlap.GetActor();
                OverlappedActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlappedActor))
            {
                // 3.获取符合要求的Actor
                OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlappedActor));
            }
        }
    }
}

void UAuraAbilitySystemLibrary::DrawDebugArc(const UObject* WorldContextObject, const FVector& CenterPos, const FVector& ForwardDir,
                                             float Radius, float ArcAngle, int32 Segments, FLinearColor Color, float Duration,
                                             float Thickness)
{
    if (!IsValid(WorldContextObject))
    {
        return;
    }

    // 从左往右绘制
    const float StartAngle = -ArcAngle * 0.5f;
    const float AngleStep = ArcAngle / Segments;

    FVector PrevPointPos = CenterPos + ForwardDir.RotateAngleAxis(StartAngle, FVector::UpVector) * Radius;
    for (int32 i = 1; i <= Segments; ++i)
    {
        const float CurrentAngle = StartAngle + AngleStep * i;
        FVector CurrentPointPos = CenterPos + ForwardDir.RotateAngleAxis(CurrentAngle, FVector::UpVector) * Radius;

        UKismetSystemLibrary::DrawDebugLine(WorldContextObject, PrevPointPos, CurrentPointPos, Color, Duration, Thickness);
        PrevPointPos = CurrentPointPos;
    }
}

void UAuraAbilitySystemLibrary::DrawDebugSector(const UObject* WorldContextObject, const FVector& CenterPos, const FVector& ForwardDir,
                                                float InnerRadius, float OuterRadius, float ArcAngle, FLinearColor Color, float Duration,
                                                float Thickness)
{
    if (!IsValid(WorldContextObject))
    {
        return;
    }

    DrawDebugArc(WorldContextObject, CenterPos, ForwardDir, OuterRadius, ArcAngle, 32, Color, Duration, Thickness);
    DrawDebugArc(WorldContextObject, CenterPos, ForwardDir, InnerRadius, ArcAngle, 32, Color, Duration, Thickness);

    FVector LeftStartPos = CenterPos + ForwardDir.RotateAngleAxis(-ArcAngle / 2.0f, FVector::UpVector) * InnerRadius;
    FVector LeftEndPos = CenterPos + ForwardDir.RotateAngleAxis(-ArcAngle / 2.0f, FVector::UpVector) * OuterRadius;
    UKismetSystemLibrary::DrawDebugLine(WorldContextObject, LeftStartPos, LeftEndPos, Color, Duration, Thickness);

    FVector RightStartPos = CenterPos + ForwardDir.RotateAngleAxis(ArcAngle / 2.0f, FVector::UpVector) * InnerRadius;
    FVector RightEndPos = CenterPos + ForwardDir.RotateAngleAxis(ArcAngle / 2.0f, FVector::UpVector) * OuterRadius;
    UKismetSystemLibrary::DrawDebugLine(WorldContextObject, RightStartPos, RightEndPos, Color, Duration, Thickness);
}

void UAuraAbilitySystemLibrary::Debug_ApplyEventBasedEffect(UAuraAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> GEClass,
                                                            const FGameplayTag& MagnitudeTag, float MagnitudeValue)
{
    static TArray<FGameplayTag> AvailableEventTags = {
        AuraGameplayTags::Attribute::Meta::IncomingXp,
        AuraGameplayTags::Attribute::Primary::Intelligence,
        AuraGameplayTags::Attribute::Primary::Resilience,
        AuraGameplayTags::Attribute::Primary::Strength,
        AuraGameplayTags::Attribute::Primary::Vigor
    };


    checkf(ASC, TEXT("[%hs] Invalid ASC!"), __FUNCTION__);
    checkf(GEClass, TEXT("[%hs] Invalid GEClass, please fill in GE_EventBasedEffect!"), __FUNCTION__);

    FGameplayEffectContextHandle GEContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle GESpecHandle = ASC->MakeOutgoingSpec(GEClass, 1.f, GEContextHandle);

    // 防止蓝图侧的GetSetbyCallerMagnitude()报错, 得对所有的EventTag设初值
    for (const FGameplayTag& EventTag : AvailableEventTags)
    {
        if (MagnitudeTag.MatchesTagExact(EventTag))
        {
            GESpecHandle.Data->SetSetByCallerMagnitude(MagnitudeTag, MagnitudeValue);
        }
        else
        {
            GESpecHandle.Data->SetSetByCallerMagnitude(EventTag, 0.f);
        }
    }

    ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data);
}
