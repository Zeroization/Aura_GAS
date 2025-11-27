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
	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
	                            AddUObject(this, &UAuraOverlayWidgetController::OnHealthChangedCallback);
	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
	                            AddUObject(this, &UAuraOverlayWidgetController::OnMaxHealthChangedCallback);
	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).
	                            AddUObject(this, &UAuraOverlayWidgetController::OnManaChangedCallback);
	AuraAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).
	                            AddUObject(this, &UAuraOverlayWidgetController::OnMaxManaChangedCallback);

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

void UAuraOverlayWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::OnManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
