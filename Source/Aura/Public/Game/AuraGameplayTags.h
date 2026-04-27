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
        }

        namespace Meta
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);           // "Attribute.Meta"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(IncomingDamage); // "Attribute.Meta.IncomingDamage"
        }
    }

    namespace GE
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);     // "GE"
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact); // "GE.HitReact"
    }

    namespace Damage
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Damage"

        namespace ElemType
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root); // "Damage.ElemType"
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire); // "Damage.ElemType.Fire"
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
    }
}
