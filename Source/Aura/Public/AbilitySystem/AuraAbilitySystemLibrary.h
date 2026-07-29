// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widget/DamageFloatingTextComponent.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UGameplayEffect;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAuraAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAuraAttributeMenuWidgetController;
class UAuraOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
#pragma region Widget Controller
    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
    static UAuraOverlayWidgetController* GetAuraOverlayWidgetController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
    static UAuraAttributeMenuWidgetController* GetAuraAttributeMenuWidgetController(const UObject* WorldContextObject);
#pragma endregion

#pragma region Character Class Defaults
    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static UCharacterClassInfo* GetEnemyCharacterClassInfo(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static void InitEnemyDefaultAttributesByClass(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC,
                                                  ECharacterClass EnemyClass, int32 EnemyLevel = 1);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static void GrantEnemyStartupAbilities(const UObject* WorldContextObject, UAuraAbilitySystemComponent* ASC,
                                           ECharacterClass CharacterClass, int32 EnemyLevel = 1);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
    static int32 GetEnemyXpRewardByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 EnemyLevel = 1);
#pragma endregion

#pragma region Gameplay Effects
    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static bool GetIsBlockedHit(const FGameplayEffectContextHandle& GEContextHandle);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static bool GetIsCriticalHit(const FGameplayEffectContextHandle& GEContextHandle);

    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, bool InIsBlockedHit);

    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, bool InIsCriticalHit);
#pragma endregion

#pragma region Damage
    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Damage")
    static bool ContainsDamageTypeByProperty(const FDamageFloatingTextProperty& Property, EAuraDamageType DamageType);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Damage")
    static bool ContainsDamageTypeByFlags(uint8 DamageTypeFlags, EAuraDamageType DamageType);

    UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|Damage")
    static bool IsFriendlyFire(AActor* First, AActor* Second);
#pragma endregion

#pragma region Environment Query
    // 获取和球重叠的所有Actor, 并且该Actor实现CombatInterface接口
    UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|Environment Query", meta = (WorldContext="WorldContextObject"))
    static void QueryActorsInSphere(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                    const TArray<AActor*> ActorsToIgnore, float SphereRadius, const FVector& SphereOrigin);
#pragma endregion

#pragma region Debug
    /// 绘制Debug用圆弧, 原理是通过插值, 在角度间绘制点并连接为线段, 这些线段会被近似为圆弧
    /// @param CenterPos    圆弧中心位置
    /// @param ForwardDir   中心位置正前方方向向量
    /// @param Radius       圆弧半径
    /// @param ArcAngle     圆弧角度
    /// @param Segments     插值的段数
    /// @param Color        圆弧颜色
    /// @param Duration     持续时间
    /// @param Thickness    线条宽度
    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Debug", meta = (WorldContext="WorldContextObject"))
    static void DrawDebugArc(const UObject* WorldContextObject, const FVector& CenterPos, const FVector& ForwardDir, float Radius,
                             float ArcAngle, int32 Segments = 32, FLinearColor Color = FLinearColor::Green, float Duration = 0.f,
                             float Thickness = 2.f);

    /// 绘制Debug用扇形
    /// @param CenterPos    扇形圆心位置
    /// @param ForwardDir   圆心正前方方向向量
    /// @param InnerRadius  内部圆弧半径
    /// @param OuterRadius  外部圆弧半径
    /// @param ArcAngle     扇形角度
    /// @param Color        扇形颜色
    /// @param Duration     持续时间
    /// @param Thickness    线条宽度
    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Debug", meta = (WorldContext="WorldContextObject"))
    static void DrawDebugSector(const UObject* WorldContextObject, const FVector& CenterPos, const FVector& ForwardDir, float InnerRadius,
                                float OuterRadius, float ArcAngle, FLinearColor Color = FLinearColor::Green, float Duration = 0.f,
                                float Thickness = 2.f);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Debug")
    static void Debug_ApplyEventBasedEffect(UAuraAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> GEClass,
                                            const FGameplayTag& MagnitudeTag, float MagnitudeValue);
#pragma endregion
};
