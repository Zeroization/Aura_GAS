// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/DataAssets/AuraAbilityInfo.h"
#include "AbilitySystem/Data/DataAssets/CharacterLevelUpInfo.h"
#include "Aura/Aura.h"
#include "Game/AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"
#include "Tools/AuraDataTableFunctionLibrary.h"


void UAuraOverlayWidgetController::OnSkillEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& SkillStatusTag,
                                                   const FGameplayTag& NewInputTag, const FGameplayTag& PrevInputTag)
{
    // 在UI层面清空输入为PrevInputTag的技能们
    FAuraAbilityData PrevAbilityData;
    PrevAbilityData.SkillStatusTag = SkillStatusTag;
    PrevAbilityData.InputTag = PrevInputTag;
    PrevAbilityData.AbilityTag = FGameplayTag::EmptyTag;
    OnAbilityInfoSet.Broadcast(PrevAbilityData);

    // 在UI层面广播装备好的技能
    FAuraAbilityData* CurData = AbilityInfo->ModifyAbilityDataByTag(AbilityTag);
    CurData->SkillStatusTag = SkillStatusTag;
    CurData->InputTag = NewInputTag;
    OnAbilityInfoSet.Broadcast(*CurData);
}

void UAuraOverlayWidgetController::BroadcastAbilityData()
{
    if (AuraAbilitySystemComponent->bStartupAbilitiesGiven)
    {
        // 如果在绑定委托前已经获取到初始GA, 直接执行对应逻辑
        BroadcastInitialAbilities();
    }
}

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
    OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
    OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
    OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
    OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
    OnLevelNumberChanged.Broadcast(AuraPlayerState->GetPlayerLevel());
}

void UAuraOverlayWidgetController::BindDelegateCallbackFunctions()
{
#pragma region PS
    // AAuraPlayerState::FOnPlayerStatChangeDelegate
    AuraPlayerState->OnPlayerXpChange.AddUObject(this, &UAuraOverlayWidgetController::OnXpChanged);
    AuraPlayerState->OnPlayerLevelChange.AddLambda([this](int32 NewLevel)
    {
        OnLevelNumberChanged.Broadcast(NewLevel);
    });
#pragma endregion

#pragma region ASC
    // UAuraOverlayWidgetController::FOnAttributeChangedSignature
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnHealthChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnMaxHealthChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnManaChanged.Broadcast(Data.NewValue);
                              });
    AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
                              .AddLambda([this](const FOnAttributeChangeData& Data)
                              {
                                  OnMaxManaChanged.Broadcast(Data.NewValue);
                              });

    // UAuraAbilitySystemComponent::FOnGetGEAssetTagDelegate
    AuraAbilitySystemComponent->OnGetGEAssetTag.AddLambda([this](const FGameplayTagContainer& AssetTags)
    {
        for (const auto& Tag : AssetTags)
        {
#ifdef WITH_EDITOR
            const FString DebugMsg = FString::Printf(TEXT("[Overlay UI Controller] GE Tag: %s"), *Tag.ToString());
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DebugMsg);
#endif

            // 只有Tag是 UI.DisplayMessage.* 的才会查表, 并将其广播至显示对应消息的UI
            if (FGameplayTag DisplayMessageTag = FGameplayTag::RequestGameplayTag(
                    AuraGameplayTags::UI::DisplayMessage::Root.GetTag().GetTagName());
                Tag.MatchesTag(DisplayMessageTag))
            {
                const FDisplayWidgetRow* Row = FAuraDataTableFunctionLibrary::GetDataTableRowByGameplayTag<FDisplayWidgetRow>(
                    MessageWidgetDataTable, Tag);
                OnDisplayMessageRowReceived.Broadcast(*Row);
            }
        }
    });

    // UAuraAbilitySystemComponent::FOnAbilitiesGivenDelegate
    AuraAbilitySystemComponent->OnAbilitiesGiven.AddUObject(this, &UAuraOverlayWidgetController::BroadcastInitialAbilities);

    // UAuraAbilitySystemComponent::FOnAbilityEquipped
    AuraAbilitySystemComponent->OnAbilityEquipped.AddUObject(this, &UAuraOverlayWidgetController::OnSkillEquipped);
#pragma endregion
}

void UAuraOverlayWidgetController::OnXpChanged(int32 NewXp)
{
    UCharacterLevelUpInfo* LevelUpInfo = AuraPlayerState->AuraLevelUpInfo;
    checkf(LevelUpInfo, TEXT("[%hs]: Can't find LevelUpInfo, please check out BP_AuraPlayerState."), __FUNCTION__);

    int32 CurLevel = LevelUpInfo->GetLevelByXP(NewXp);
    int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num() - 1;
    float XpBarPercent;
    if (CurLevel <= MaxLevel && CurLevel != INDEX_NONE)
    {
        int32 PrevLevelRequireXp = (CurLevel >= 1) ? LevelUpInfo->LevelUpInfos[CurLevel - 1].LevelUpRequirement : 0;
        int32 CurLevelRequireXP = LevelUpInfo->LevelUpInfos[CurLevel].LevelUpRequirement;

        int32 XpBarCurXp = NewXp - PrevLevelRequireXp;
        int32 XpBarDeltaXp = CurLevelRequireXP - PrevLevelRequireXp;
        XpBarPercent = static_cast<float>(XpBarCurXp) / static_cast<float>(XpBarDeltaXp);
    }
    else
    {
        XpBarPercent = 1.f;
    }

    OnXpPercentChanged.Broadcast(XpBarPercent);
}
