// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/NPCAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Player/Character/BOCharacter.h"

ANPCAIController::ANPCAIController()
{
	NPCBlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName(TEXT("NPC Blackboard")));
}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();

	SetBlackboarValues();
	StartBehaviorTree();
}

void ANPCAIController::SetBlackboarValues()
{
	if (!NPCBlackboardComp || !GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	ABOCharacter* Player = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>();
	if (!Player)
	{
		return;
	}

	NPCBlackboardComp->SetValueAsBool(TEXT("IsActing"), false);
	NPCBlackboardComp->SetValueAsBool(TEXT("IsInteracting"), false);
	NPCBlackboardComp->SetValueAsBool(TEXT("IsLookingAtPlayer"), false);
	NPCBlackboardComp->SetValueAsObject(TEXT("Player"), Player);
}

void ANPCAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree started"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Behavior Tree Asset"));
	}
}

UBlackboardComponent* ANPCAIController::GetNPCBlackboardComp() const
{
	return NPCBlackboardComp;
}
