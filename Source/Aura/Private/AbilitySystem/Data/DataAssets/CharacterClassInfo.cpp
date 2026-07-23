// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/DataAssets/CharacterClassInfo.h"


FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass ClassEnum)
{
    return CharacterClassInfoMap.FindChecked(ClassEnum);
}
