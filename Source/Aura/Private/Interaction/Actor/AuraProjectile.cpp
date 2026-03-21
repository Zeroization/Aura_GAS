// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Actor/AuraProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Character/AuraCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	LoopSoundComponent = CreateDefaultSubobject<UAudioComponent>("LoopSound");
	LoopSoundComponent->bStopWhenOwnerDestroyed = true;
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(ProjectileLifeSpan);

	// 防止投射物撞到发射它的网格体
	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);
	if (AAuraCharacter* InstigatorCharacter = Cast<AAuraCharacter>(GetInstigator()))
	{
		InstigatorCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);
		InstigatorCharacter->GetMesh()->IgnoreActorWhenMoving(this, true);
	}

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereBeginOverlap);

	LoopSoundComponent->Play();
}

void AAuraProjectile::Destroyed()
{
	if (!bIsImpactSFXPlayed && !HasAuthority())
	{
		PlayImpactSFX();
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayImpactSFX();

	if (HasAuthority())
	{
		// 服务器逻辑
		Destroy();
	}
	else
	{
		// 客户端逻辑
		bIsImpactSFXPlayed = true;
	}
}

void AAuraProjectile::PlayImpactSFX()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}
