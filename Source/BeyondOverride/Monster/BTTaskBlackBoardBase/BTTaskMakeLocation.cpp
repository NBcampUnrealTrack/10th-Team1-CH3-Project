// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskMakeLocation.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskMakeLocation::UBTTaskMakeLocation()
{
	NodeName = TEXT("Make Location Patrol Point");
}

EBTNodeResult::Type UBTTaskMakeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	float CanNotFind = 5000.0f;
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

	AMonsterCharacter* AIMonster = Cast<AMonsterCharacter>(AIController->GetPawn());
	if (!AIMonster)
	{
		return EBTNodeResult::Failed;
	}

	ABOCharacter* Target = AIController->GetTarget();
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector LocationPoint = AIController->GetTargetPoint();

	float TargetDist = FVector::Distance(Target->GetActorLocation(), AIMonster->GetActorLocation());

	if (TargetDist > CanNotFind || LocationPoint == FVector::ZeroVector)
	{
		TargetCenter = LocationPoint;
	}
	else
	{
		TargetCenter = Target->GetActorLocation();
	}

	TargetCenter.X += FMath::RandRange(-RangeRand, RangeRand);
	TargetCenter.Y += FMath::RandRange(-RangeRand, RangeRand);

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	if (NavSystem && NavSystem->ProjectPointToNavigation(TargetCenter, NavLocation))
	{
		PatrolPoint = NavLocation.Location;
	}

	BlackboardComp->SetValueAsVector(TEXT("LocationPoint"), PatrolPoint);
	return EBTNodeResult::Succeeded;
}
