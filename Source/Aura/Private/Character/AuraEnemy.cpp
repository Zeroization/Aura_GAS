// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"


void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("[ %s ]: I am highlighted!"), *GetActorNameOrLabel());
}

void AAuraEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("[ %s ]: I am unhighlighted!"), *GetActorNameOrLabel())
}
