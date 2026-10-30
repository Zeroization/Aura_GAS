// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace AuraGameplayTags
{
    namespace Attribute
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);  // "Attribute"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Point); // "Attribute.Point"

        namespace Vital
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);   // "Attribute.Vital"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health); // "Attribute.Vital.Health"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana);   // "Attribute.Vital.Mana"
        }

        namespace Primary
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);         // "Attribute.Primary"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Intelligence); // "Attribute.Primary.Intelligence"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resilience);   // "Attribute.Primary.Resilience"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Strength);     // "Attribute.Primary.Strength"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Vigor);        // "Attribute.Primary.Vigor"
        }

        namespace Secondary
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);                   // "Attribute.Secondary"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaRegeneration);       // "Attribute.Secondary.ManaRegeneration"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana);                // "Attribute.Secondary.MaxMana"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Armor);                  // "Attribute.Secondary.Armor"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockChance);            // "Attribute.Secondary.BlockChance"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitResistance);  // "Attribute.Secondary.CriticalHitResistance"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArmorPenetration);       // "Attribute.Secondary.ArmorPenetration"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitChance);      // "Attribute.Secondary.CriticalHitChance"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(CriticalHitBonusDamage); // "Attribute.Secondary.CriticalHitBonusDamage"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegeneration);     // "Attribute.Secondary.HealthRegeneration"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth);              // "Attribute.Secondary.MaxHealth"

            namespace ElemResistance
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);      // "Attribute.Secondary.ElemResistance"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire);      // "Attribute.Secondary.ElemResistance.Fire"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lightning); // "Attribute.Secondary.ElemResistance.Lightning"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arcane);    // "Attribute.Secondary.ElemResistance.Arcane"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Physical);  // "Attribute.Secondary.ElemResistance.Physical"
            }
        }

        namespace Meta
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);           // "Attribute.Meta"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(IncomingDamage); // "Attribute.Meta.IncomingDamage"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(IncomingXp);     // "Attribute.Meta.IncomingXp"
        }
    }

    namespace GE
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);     // "GE"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact); // "GE.HitReact"
    }

    namespace GA
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "GA"
        namespace Skill
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);   // "GA.Skill"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack); // "GA.Skill.Attack"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Summon); // "GA.Skill.Summon"

            namespace Fire
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);     // "GA.Skill.Fire"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireBolt); // "GA.Skill.Fire.FireBolt"
            }

            namespace Lightning
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);        // "GA.Skill.Lightning"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Electrocute); // "GA.Skill.Lightning.Electrocute"
            }

            namespace Passive
            {
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);          // "GA.Skill.Passive"
                UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListenForEvent) // "GA.Skill.Passive.ListenForEvent"
            }
        }

        namespace Cooldown
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);     // "GA.Cooldown"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireBolt); // "GA.Cooldown.FireBolt"
        }

        namespace SkillStatus
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);     // "GA.SkillStatus"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locked);   // "GA.SkillStatus.Locked"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Eligible); // "GA.SkillStatus.Eligible"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unlocked); // "GA.SkillStatus.Unlocked"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipped); // "GA.SkillStatus.Equipped"
        }

        namespace SkillType
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);      // "GA.SkillType"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Offensive); // "GA.SkillType.Offensive"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Passive);   // "GA.SkillType.Passive"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);      // "GA.SkillType.None"
        }
    }

    namespace Damage
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Damage"

        namespace ElemType
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);      // "Damage.ElemType"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire);      // "Damage.ElemType.Fire"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lightning); // "Damage.ElemType.Lightning"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arcane);    // "Damage.ElemType.Arcane"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Physical);  // "Damage.ElemType.Physical"

            // 元素伤害类型Tag <-> 元素抵抗类型Tag
            const TMap<FGameplayTag, FGameplayTag>& GetElemTypeToResistanceMap();
        }
    }

    namespace Debuff
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);                // "Debuff"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Burn);                // "Debuff.Burn"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stun);                // "Debuff.Stun"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArcanePlaceholder);   // "Debuff.ArcanePlaceholder"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(PhysicalPlaceholder); // "Debuff.PhysicalPlaceholder"

        // Debuff总个数
        constexpr int8 MaxDebuffNum = 4;
        // DebuffTag <-> 和它强相关的元素伤害类型Tag
        const TMap<FGameplayTag, FGameplayTag>& GetDebuffToElemTypeMap();
    }

    namespace CombatSocket
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);   // "CombatSocket"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon); // "CombatSocket.Weapon"

        namespace SelfMesh
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);      // "CombatSocket.SelfMesh"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(LeftHand);  // "CombatSocket.SelfMesh.LeftHand"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(RightHand); // "CombatSocket.SelfMesh.RightHand"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tail);      // "CombatSocket.SelfMesh.Tail"
        }
    }

    namespace Montage
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Montage"

        namespace Attack
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);  // "Montage.Attack"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot1); // "Montage.Attack.Slot1"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot2); // "Montage.Attack.Slot2"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot3); // "Montage.Attack.Slot3"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot4); // "Montage.Attack.Slot4"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot5); // "Montage.Attack.Slot5"
        }
    }

    namespace UI
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "UI"

        namespace DisplayMessage
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);          // "UI.DisplayMessage"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthCrystal); // "UI.DisplayMessage.HealthCrystal"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthPotion);  // "UI.DisplayMessage.HealthPotion"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaCrystal);   // "UI.DisplayMessage.ManaCrystal"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaPotion);    // "UI.DisplayMessage.ManaPotion"
        }
    }

    namespace Input
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Input"

        namespace Mouse
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Input.Mouse"			
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(LMB);  // "Input.Mouse.LMB"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(RMB);  // "Input.Mouse.RMB"
        }

        namespace Keyboard
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);  // "Input.Keyboard"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_1); // "Input.Keyboard.1"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_2); // "Input.Keyboard.2"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_3); // "Input.Keyboard.3"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_4); // "Input.Keyboard.4"
        }

        namespace Passive
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);  // "Input.Passive"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_1); // "Input.Passive.1"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Num_2); // "Input.Passive.2"
        }
    }
}
