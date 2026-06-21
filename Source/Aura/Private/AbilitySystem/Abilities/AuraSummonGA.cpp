// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonGA.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


TArray<FVector> UAuraSummonGA::GetSpawnLocations()
{
    const AActor* AvatarActor = GetAvatarActorFromActorInfo();
    const FVector AvatarActorForwardDir = AvatarActor->GetActorForwardVector();
    const FVector AvatarActorLocation = AvatarActor->GetActorLocation();

    if (bEnableDebug)
    {
        UAuraAbilitySystemLibrary::DrawDebugSector(AvatarActor, AvatarActorLocation, AvatarActorForwardDir, MinSpawnDistance,
                                                   MaxSpawnDistance, SpawnSpreadAngle, FLinearColor::Green, 3.f);
    }

    TArray<FVector> SpawnLocations;

    // 使用插值思想, 在范围内随机生成召唤物
    const float StepSpawnSpreadAngle = SpawnSpreadAngle / (NumOfMinions + 1.f);
    FVector StartSpawnDir = AvatarActorForwardDir.RotateAngleAxis(-SpawnSpreadAngle / 2.f, FVector::UpVector);
    for (int32 i = 1; i <= NumOfMinions; ++i)
    {
        FVector CurrSpawnDir = StartSpawnDir.RotateAngleAxis(StepSpawnSpreadAngle * i, FVector::UpVector);
        FVector CurrSpawnPos = AvatarActorLocation + CurrSpawnDir * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

        // 进行垂直射线检测, 确保召唤物被生成在地面上
        FHitResult Hit;
        GetWorld()->LineTraceSingleByChannel(Hit, CurrSpawnPos + FVector(0.f, 0.f, 400.f),
                                             CurrSpawnPos - FVector(0.f, 0.f, 400.f), ECC_Visibility);
        if (Hit.bBlockingHit)
        {
            CurrSpawnPos = Hit.ImpactPoint + FVector(0.f, 0.f, 70.f);
        }

        SpawnLocations.Add(CurrSpawnPos);

        if (bEnableDebug)
        {
            UKismetSystemLibrary::DrawDebugLine(AvatarActor,
                                                AvatarActorLocation + CurrSpawnDir * MinSpawnDistance,
                                                AvatarActorLocation + CurrSpawnDir * MaxSpawnDistance,
                                                FLinearColor::Red, 3.f, 2.f);
            UKismetSystemLibrary::DrawDebugSphere(AvatarActor, CurrSpawnPos, 10.f, 12, FLinearColor::Red,
                                                  3.f, 2.5f);
        }
    }

    return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonGA::GetRandomMinionClass()
{
    return MinionClassArray[FMath::RandRange(0, MinionClassArray.Num() - 1)];
}
