// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskMakePoint.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskMakePoint::UBTTaskMakePoint()
{
	NodeName = TEXT("Make Patrol Point");
}

EBTNodeResult::Type UBTTaskMakePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float RangeRand;

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

	AMonsterCharacter* Monster = AIController->GetMonster();
	if (!Monster)
	{
		return EBTNodeResult::Failed;
	}

	EPatrolType PatrolType = AIController->IsPatrolType();

	EPointPatrolState NowState = AIController->NowPatrolState();
	AIController->ChangePatrolState();

	if (PatrolType == EPatrolType::Random)
	{
		RangeRand = 750.0f;
		TargetCenter = AIController->GetSpawnPoint();
		TargetCenter.X = TargetCenter.X + FMath::RandRange(-RangeRand, RangeRand);
		TargetCenter.Y = TargetCenter.Y + FMath::RandRange(-RangeRand, RangeRand);
	}
	else if (PatrolType == EPatrolType::Point)
	{
		TargetCenter = AIController->GetSpawnPoint();
		TargetCenter.X = TargetCenter.X + AIController->GetPointX();
		TargetCenter.Y = TargetCenter.Y + AIController->GetPointY();
	}
	else
	{
		TargetCenter = AIController->GetSpawnPoint();
	}

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	if (NavSystem && NavSystem->ProjectPointToNavigation(TargetCenter, NavLocation))
	{
		PatrolPoint = NavLocation.Location;
	}

	BlackboardComp->SetValueAsVector(TEXT("PatrolPoint"), PatrolPoint);
	return EBTNodeResult::Succeeded;
}
