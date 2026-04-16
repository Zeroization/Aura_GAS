// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"


UAuraOverlayWidgetController* UAuraAbilitySystemLibrary::GetAuraOverlayWidgetController(const UObject* WorldContextObject)
{
    // 从本地玩家获取UI Controller
    if (AAuraPlayerController* PlayerController =
        Cast<AAuraPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
        {
            AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
            UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
            UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());
            const FAuraWidgetControllerParams Params(PlayerController, PlayerState, ASC, AttributeSet);
            return AuraHUD->GetAuraOverlayWidgetController(Params);
        }
    }

    return nullptr;
}

UAuraAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAuraAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    // 从本地玩家获取UI Controller
    if (AAuraPlayerController* PlayerController =
        Cast<AAuraPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
        {
            AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
            UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
            UAuraAttributeSet* AttributeSet = Cast<UAuraAttributeSet>(PlayerState->GetAttributeSet());
            const FAuraWidgetControllerParams Params(PlayerController, PlayerState, ASC, AttributeSet);
            return AuraHUD->GetAuraAttributeMenuWidgetController(Params);
        }
    }

    return nullptr;
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetEnemyCharacterClassInfo(const UObject* WorldContextObject)
{
    if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
    {
        return AuraGameMode->EnemyCharacterClassInfo;
    }

    return nullptr;
}

void UAuraAbilitySystemLibrary::InitEnemyDefaultAttributesByClass(const UObject* WorldContextObject,
                                                                  ECharacterClass EnemyClass, float Level,
                                                                  UAuraAbilitySystemComponent* ASC)
{
    if (UCharacterClassInfo* EnemyClassInfo = GetEnemyCharacterClassInfo(WorldContextObject))
    {
        FCharacterClassDefaultInfo EnemyClassDefaultInfo = EnemyClassInfo->GetClassDefaultInfo(EnemyClass);

        AActor* AvatarActor = ASC->GetAvatarActor();
        // Primary Attributes
        FGameplayEffectContextHandle PrimaryAttrGEContextHandle = ASC->MakeEffectContext();
        PrimaryAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle PrimaryAttrGESpecHandle = ASC->MakeOutgoingSpec(EnemyClassDefaultInfo.PrimaryAttributes, Level,
                                                                                        PrimaryAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttrGESpecHandle.Data);

        // Secondary Attributes
        FGameplayEffectContextHandle SecondaryAttrGEContextHandle = ASC->MakeEffectContext();
        SecondaryAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle SecondaryAttrGESpecHandle = ASC->MakeOutgoingSpec(
            EnemyClassInfo->SecondaryAttributes, Level, SecondaryAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttrGESpecHandle.Data);

        // Vital Attributes
        FGameplayEffectContextHandle VitalAttrGEContextHandle = ASC->MakeEffectContext();
        VitalAttrGEContextHandle.AddSourceObject(AvatarActor);
        const FGameplayEffectSpecHandle VitalAttrGESpecHandle = ASC->MakeOutgoingSpec(
            EnemyClassInfo->VitalAttributes, Level, VitalAttrGEContextHandle);
        ASC->ApplyGameplayEffectSpecToSelf(*VitalAttrGESpecHandle.Data);
    }
}

void UAuraAbilitySystemLibrary::GrantEnemyStartupAbilities(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC)
{
    if (UCharacterClassInfo* EnemyClassInfo = GetEnemyCharacterClassInfo(WorldContextObject))
    {
        for (const auto& AbilityClass : EnemyClassInfo->CommonAbilities)
        {
            FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(AbilityClass, 1);
            ASC->GiveAbility(GASpec);
        }
    }
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
