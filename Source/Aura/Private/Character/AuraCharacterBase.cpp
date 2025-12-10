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

void AAuraCharacterBase::InitPrimaryAttributes() const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("[%hs] ASC is null!"), __FUNCTION__);
	checkf(IsValid(InitPrimaryAttrGEClass), TEXT("[%hs] InitPrimaryAttrGEClass is null, plz fill it in editor"),
	       __FUNCTION__);

	const FGameplayEffectContextHandle GEContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle GESpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		InitPrimaryAttrGEClass, 1.f, GEContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data, GetAbilitySystemComponent());
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
