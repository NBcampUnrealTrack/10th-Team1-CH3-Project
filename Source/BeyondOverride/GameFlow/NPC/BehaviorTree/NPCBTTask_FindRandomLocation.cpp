// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/BehaviorTree/NPCBTTask_FindRandomLocation.h"

#include "NavigationSystem.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFlow/NPC/NPCAIController.h"

UNPCBTTask_FindRandomLocation::UNPCBTTask_FindRandomLocation()
{
	NodeName = TEXT("NPC Find Random Location");

	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UNPCBTTask_FindRandomLocation, LocationKey));
}

EBTNodeResult::Type UNPCBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* NPC = AIController->GetPawn();
	if (!NPC)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation RandomLocation;
	bool IsFound = NavSystem->GetRandomReachablePointInRadius(NPC->GetActorLocation(), SearchRadius, RandomLocation);

	if (IsFound)
	{
		if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
		{
			BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
