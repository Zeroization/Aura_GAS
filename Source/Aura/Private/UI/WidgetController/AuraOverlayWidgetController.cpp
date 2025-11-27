// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UAuraOverlayWidgetController::BindDelegateCallbackFunctions()
{
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

			// 只有Tag是 DisplayMessage.* 的才会查表, 并将其广播至显示对应消息的UI
			if (FGameplayTag DisplayMessageTag = FGameplayTag::RequestGameplayTag("DisplayMessage");
				Tag.MatchesTag(DisplayMessageTag))
			{
				const FDisplayWidgetRow* Row = GetDataTableRowByGameplayTag<FDisplayWidgetRow>(
					MessageWidgetDataTable, Tag);
				OnDisplayMessageRowReceived.Broadcast(*Row);
			}
		}
	});
}
