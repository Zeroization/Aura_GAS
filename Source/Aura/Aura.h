// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define OUTLINE_COLOR_RED	250
#define OUTLINE_COLOR_BLUE	251
#define OUTLINE_COLOR_WHITE 252

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType, meta=(Bitflags))
enum class EAuraDamageType : uint8
{
    None = 0,          // 0b00000000
    Normal = 1 << 0,   // 0b00000001
    Blocked = 1 << 1,  // 0b00000010
    Critical = 1 << 2, // 0b00000100
};
