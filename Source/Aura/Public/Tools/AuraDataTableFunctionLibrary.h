// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
class AURA_API FAuraDataTableFunctionLibrary
{
public:
    /**
     * 根据 GameplayTag 进行查表的工具函数, 行名必须是 GameplayTag 名
     * @tparam DTRowType 任意数据表格的行类型
     * @param DataTable 数据表格指针
     * @param Tag Gameplay Tag
     * @return 指向表行数据的指针
     */
    template <typename DTRowType>
    static DTRowType* GetDataTableRowByGameplayTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename DTRowType>
DTRowType* FAuraDataTableFunctionLibrary::GetDataTableRowByGameplayTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
    return DataTable->FindRow<DTRowType>(Tag.GetTagName(), TEXT(""));
}
