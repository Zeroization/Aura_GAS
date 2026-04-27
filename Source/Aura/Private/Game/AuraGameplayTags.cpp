// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameplayTags.h"

namespace AuraGameplayTags
{
    namespace Attribute
    {
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                       "Attribute",
                                       "GAS中用到的属性; 此Tag仅用于Tag相关操作(如Matches)");
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Point,
                                       "Attribute.Point",
                                       "用于给首要属性加点的属性点");

        namespace Vital
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Attribute.Vital",
                                           "Vital Attributes 必备属性; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Health,
                                           "Attribute.Vital.Health",
                                           "生命值 HP");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mana,
                                           "Attribute.Vital.Mana",
                                           "魔法值 MP")
        }

        namespace Primary
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Attribute.Primary",
                                           "Primary Attributes 首要属性: 值独立设置, 不依赖其他属性; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Strength,
                                           "Attribute.Primary.Strength",
                                           "力量STR: 增加物理攻击伤害");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Intelligence,
                                           "Attribute.Primary.Intelligence",
                                           "智慧INT: 增加魔法攻击伤害和持续回蓝 ManaRegeneration");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Vigor,
                                           "Attribute.Primary.Vigor",
                                           "活力VIG: 增加生命值 Health 和持续回血 HealthRegeneration");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resilience,
                                           "Attribute.Primary.Resilience",
                                           "抗性RES: 增加护甲值 Armor 和护甲穿透 ArmorPenetration");
        }

        namespace Secondary
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Attribute.Secondary",
                                           "Secondary Attributes 次级属性: 值有依赖一级属性或部分次级属性; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaRegeneration,
                                           "Attribute.Secondary.ManaRegeneration",
                                           "Mana Regeneration: 依赖属性INT, 每秒持续回复特定蓝值");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxMana,
                                           "Attribute.Secondary.MaxMana",
                                           "Max Mana: 依赖属性INT, Mana的最大值");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Armor,
                                           "Attribute.Secondary.Armor",
                                           "Armor: 依赖属性RES, 减少收到的伤害, 增加格挡率 BlockChance");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(BlockChance,
                                           "Attribute.Secondary.BlockChance",
                                           "Block Chance: 依赖属性Armor, 概率使受到伤害减半");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalHitResistance,
                                           "Attribute.Secondary.CriticalHitResistance",
                                           "Critical Hit Resistance: 依赖属性Armor, 减少来自敌人攻击的暴击率");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(ArmorPenetration,
                                           "Attribute.Secondary.ArmorPenetration",
                                           "Armor Penetration: 依赖属性RES, 忽略敌人部分护甲值, 增加暴击率 CriticalHitChance");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalHitChance,
                                           "Attribute.Secondary.CriticalHitChance",
                                           "Critical Hit Chance: 依赖属性ArmorPenetration, 概率造成双倍伤害+暴击补偿伤害CriticalHitBonusDamage");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(CriticalHitBonusDamage,
                                           "Attribute.Secondary.CriticalHitBonusDamage",
                                           "Critical Hit Bonus Damage: 依赖属性ArmorPenetration, 暴击补偿伤害");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegeneration,
                                           "Attribute.Secondary.HealthRegeneration",
                                           "Health Regeneration: 依赖属性VIG, 每秒持续回复特定生命值");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxHealth,
                                           "Attribute.Secondary.MaxHealth",
                                           "Max Health: 依赖属性VIG, Health的最大值");
        }

        namespace Meta
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Attribute.Meta",
                                           "Meta Attributes 元属性: 用于传递过程数据的临时属性; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(IncomingDamage,
                                           "Attribute.Meta.IncomingDamage",
                                           "当前角色在本次受到的总伤害, 在AttributeSet中统一处理");
        }
    }

    namespace GE
    {
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                       "GE",
                                       "实现的Gameplay Effect; 此Tag仅用于Tag相关操作(如Matches)");
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact,
                                       "GE.HitReact",
                                       "当GE HitReact被施加时, 赋予目标Target");
    }

    namespace Damage
    {
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                       "Damage",
                                       "伤害分类; 此Tag仅用于Tag相关操作(如Matches)");

        namespace ElemType
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Damage.ElemType",
                                           "伤害元素类型; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fire,
                                           "Damage.ElemType.Fire",
                                           "火元素伤害");
        }
    }

    namespace UI
    {
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                       "UI",
                                       "UI用到的Tag; 此Tag仅用于Tag相关操作(如Matches)");

        namespace DisplayMessage
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "UI.DisplayMessage",
                                           "用于提示信息UI的Tag; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthCrystal,
                                           "UI.DisplayMessage.HealthCrystal",
                                           "生命水晶的相关提示信息");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthPotion,
                                           "UI.DisplayMessage.HealthPotion",
                                           "生命药水的相关提示信息");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaCrystal,
                                           "UI.DisplayMessage.ManaCrystal",
                                           "魔法水晶的相关提示信息");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaPotion,
                                           "UI.DisplayMessage.ManaPotion",
                                           "魔法药水的相关提示信息");
        }
    }

    namespace Input
    {
        UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                       "Input",
                                       "玩家输入用到的Tag; 此Tag仅用于Tag相关操作(如Matches)");

        namespace Mouse
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Input.Mouse",
                                           "鼠标输入用到的Tag; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(LMB,
                                           "Input.Mouse.LMB",
                                           "鼠标左键");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(RMB,
                                           "Input.Mouse.RMB",
                                           "鼠标右键");
        }

        namespace Keyboard
        {
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,
                                           "Input.Keyboard",
                                           "键盘输入用到的Tag; 此Tag仅用于Tag相关操作(如Matches)");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Num_1,
                                           "Input.Keyboard.1",
                                           "1");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Num_2,
                                           "Input.Keyboard.2",
                                           "2");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Num_3,
                                           "Input.Keyboard.3",
                                           "3");
            UE_DEFINE_GAMEPLAY_TAG_COMMENT(Num_4,
                                           "Input.Keyboard.4",
                                           "4");
        }
    }
}
