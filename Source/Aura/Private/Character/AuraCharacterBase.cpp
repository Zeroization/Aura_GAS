// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Game/AuraGameplayTags.h"
#include "Kismet/GameplayStatics.h"


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

void AAuraCharacterBase::MulticastOnCharacterDeath_Implementation()
{
    bIsDead = true;

    // 播放死亡音效
    UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

    // 设定武器掉落相关参数
    Weapon->SetSimulatePhysics(true);
    Weapon->SetEnableGravity(true);
    Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    // 设定角色网格体Ragdoll效果
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetEnableGravity(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 角色和武器溶解
    Dissolve();
}

void AAuraCharacterBase::Die()
{
    // 角色死亡时武器会掉落
    Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

    MulticastOnCharacterDeath();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
    FName SocketName = SocketTagToName[SocketTag];
    checkf(!SocketName.IsNone(), TEXT("Socket name is none!"));

    if (SocketTag.MatchesTagExact(AuraGameplayTags::CombatSocket::Weapon) && IsValid(Weapon))
    {
        return Weapon->GetSocketLocation(SocketName);
    }
    if (SocketTag.MatchesTag(AuraGameplayTags::CombatSocket::SelfMesh::Root))
    {
        return GetMesh()->GetSocketLocation(SocketName);
    }

    checkf(false, TEXT("Should NOT go here."));
    return {};
}

FAuraTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
    FAuraTaggedMontage* TaggedMontage = AttackMontages.FindByPredicate([&MontageTag](const FAuraTaggedMontage& Elem) -> bool
    {
        return Elem.MontageTag.MatchesTagExact(MontageTag);
    });
    checkf(TaggedMontage != nullptr, TEXT("[%s]: Can't find FAuraTaggedMontage by tag [ %s ]"), ANSI_TO_TCHAR(__FUNCTION__),
           *MontageTag.GetTagName().ToString());

    return *TaggedMontage;
}

TArray<FAuraTaggedMontage> AAuraCharacterBase::GetTaggedAttackMontages_Implementation()
{
    return AttackMontages;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
    return HitReactMontage;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
    return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
    return this;
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
    return MinionCount;
}

void AAuraCharacterBase::IncreaseMinionCountByAmount_Implementation(int32 Amount)
{
    MinionCount += Amount;
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

UNiagaraSystem* AAuraCharacterBase::GetImpactBloodEffect_Implementation()
{
    return BloodImpactEffect;
}

void AAuraCharacterBase::Dissolve()
{
    TArray<UMaterialInstanceDynamic*> DynamicDissolveMIs;
    if (IsValid(CharacterDissolveMaterialInstance))
    {
        // MaterialInstanceDynamic: 可以在运行时计算（和编辑）。
        UMaterialInstanceDynamic* MeshDynamicMI = UMaterialInstanceDynamic::Create(CharacterDissolveMaterialInstance, this);
        GetMesh()->SetMaterial(0, MeshDynamicMI);
        DynamicDissolveMIs.Add(MeshDynamicMI);
    }
    if (IsValid(WeaponDissolveMaterialInstance))
    {
        UMaterialInstanceDynamic* WeaponDynamicMI = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
        Weapon->SetMaterial(0, WeaponDynamicMI);
        DynamicDissolveMIs.Add(WeaponDynamicMI);
    }
    StartDissolveTimeline(DynamicDissolveMIs);
}

void AAuraCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}
