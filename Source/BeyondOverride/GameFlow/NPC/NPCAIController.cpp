// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

ANPCAIController::ANPCAIController()
{
	NPCBlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName(TEXT("NPC Blackboard")));
}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();

	if (NPCBlackboardComp)
	{
		// set values
		NPCBlackboardComp->SetValueAsBool(TEXT("IsActing"), false);
	}
}

UBlackboardComponent* ANPCAIController::GetNPCBlackboardComp() const
{
	return NPCBlackboardComp;
}
