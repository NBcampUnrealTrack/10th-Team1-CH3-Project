// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/BehaviorTree/NPCBTTask_LookAtPlayer.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFlow/NPC/NPCAIController.h"
#include "Logging/BOLog.h"

UNPCBTTask_LookAtPlayer::UNPCBTTask_LookAtPlayer()
{
	NodeName = TEXT("NPC Look At Player");

	bNotifyTick = true;
}

void UNPCBTTask_LookAtPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* NPC = OwnerComp.GetAIOwner()->GetPawn();

	if (!NPC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	AActor* Player = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("Player")));

	if (!Player)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector Direction = Player->GetActorLocation() - NPC->GetActorLocation();

	Direction.Z = 0.f;

	FRotator TargetRotation = Direction.Rotation();

	if (NPC->GetActorRotation().Equals(TargetRotation, 1.0f))
	{
		BlackboardComponent->SetValueAsBool(TEXT("IsLookingAtPlayer"), true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FRotator NewRotation = FMath::RInterpTo(NPC->GetActorRotation(), TargetRotation, DeltaSeconds, RotationSpeed);

	NPC->SetActorRotation(NewRotation);
}

EBTNodeResult::Type UNPCBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}
