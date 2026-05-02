// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AAuraAIController::AAuraAIController()
{
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree");
}

void AAuraAIController::BeginPlay()
{
    Super::BeginPlay();
}
