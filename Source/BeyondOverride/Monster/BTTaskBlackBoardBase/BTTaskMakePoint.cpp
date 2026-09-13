// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskMakePoint.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskMakePoint::UBTTaskMakePoint()
{
	NodeName = TEXT("Make Patrol Point");
}

EBTNodeResult::Type UBTTaskMakePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float RangeRand = 750.0f;

	FVector PatrolPoint;
	FVector TargetCenter;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterCharacter* Monster = Cast<AMonsterCharacter>(AIController->GetPawn());
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	UStateComponent* MonsterState = Monster->GetState();
	if (!MonsterState)
	{
		return EBTNodeResult::Failed;
	}

	TargetCenter = MonsterState->GetSpawnPoint();
	TargetCenter.X += FMath::RandRange(-RangeRand, RangeRand);
	TargetCenter.Y += FMath::RandRange(-RangeRand, RangeRand);

	UNavigationSystemV1* NavSystem =
		FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	if (NavSystem &&
		NavSystem->ProjectPointToNavigation(TargetCenter, NavLocation))
	{
		PatrolPoint = NavLocation.Location;
	}

	BlackboardComp->SetValueAsVector(TEXT("RandPoint"), PatrolPoint);
	return EBTNodeResult::Succeeded;
}
