// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	/// Begin Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	/// End Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilitySystem()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GEClass, float Level) const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("[%hs] Self ASC is null!"), __FUNCTION__);
	checkf(IsValid(GEClass), TEXT("[%hs] GEClass is null, plz check fillings"), __FUNCTION__);

	FGameplayEffectContextHandle GEContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GEContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GESpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GEClass, Level, GEContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data, GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(InitPrimaryAttrGEClass);
	ApplyEffectToSelf(InitSecondaryAttrGEClass);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
