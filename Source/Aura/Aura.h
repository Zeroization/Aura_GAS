// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

#pragma region Core
// 自定义日志分类
DECLARE_LOG_CATEGORY_EXTERN(LogAuraGame, Log, All);

// 自定义碰撞通道
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#pragma endregion

#pragma region Render
// 描边颜色
#define OUTLINE_COLOR_RED	250
#define OUTLINE_COLOR_BLUE	251
#define OUTLINE_COLOR_WHITE 252
#pragma endregion

#pragma region Gameplay
// Actor FName Tag
#define AURA_ACTOR_FNAME_TAG_PLAYER "Player"
#define AURA_ACTOR_FNAME_TAG_ENEMY "Enemy"

UENUM(BlueprintType, meta=(Bitflags))
enum class EAuraDamageType : uint8
{
    None = 0,          // 0b00000000
    Normal = 1 << 0,   // 0b00000001
    Blocked = 1 << 1,  // 0b00000010
    Critical = 1 << 2, // 0b00000100
};
#pragma endregion