// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	/// Begin Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	/// End Category: Combat <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	/// Begin: Animation
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("Motion Warping");
	/// End: Animation 
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AuraAbilitySystemComponent;
}

FVector AAuraCharacterBase::GetProjectileSpawnLocation()
{
	checkf(IsValid(Weapon), TEXT("[%hs]: Character [%s] 's weapon is null!!!"), __FUNCTION__, *GetNameSafe(this));
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::InitAbilitySystem()
{
}

void AAuraCharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(InitPrimaryAttrGEClass);
	ApplyEffectToSelf(InitSecondaryAttrGEClass);
	ApplyEffectToSelf(InitVitalAttrGEClass);
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

void AAuraCharacterBase::GrantCharacterStartupAbilities()
{
	// 只有在服务端才能对GA进行赋予/撤销操作
	if (!HasAuthority())
	{
		return;
	}

	AuraAbilitySystemComponent->GrantActorGA(StartupAbilities);
}

bool AAuraCharacterBase::CheckMotionWarpTargetExists(const FName& WarpTargetName)
{
	return MotionWarping->FindWarpTarget(WarpTargetName) != nullptr;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
