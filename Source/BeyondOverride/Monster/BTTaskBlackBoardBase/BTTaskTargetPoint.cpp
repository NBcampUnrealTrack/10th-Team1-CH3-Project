// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskTargetPoint.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskTargetPoint::UBTTaskTargetPoint()
{
	NodeName = TEXT("Make Target Point");
}

EBTNodeResult::Type UBTTaskTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
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

	UAttackDataComponent* AIAttackData = AIMonster->GetAttackDataComponent();
	if (!AIAttackData)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Target = Cast<APawn>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	FVector TargetLocation = Target->GetActorLocation();

	FVector MoveLocation;

	float AttackRange = AIAttackData->GetAttackRange() * AIAttackData->GetAttackRange();
	float TargetDistance = FVector::DistSquared(Target->GetActorLocation(), AIMonster->GetActorLocation());

	float BaseAngle = (AIMonster->GetActorLocation() - TargetLocation).Rotation().Yaw;

	if (NavSystem && (!NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation) || TargetDistance < AttackRange))
	{

		float Radius = AIAttackData->GetAttackRange() - (AIAttackData->GetAttackRange() / 10);

		for (int i = 0; i < 36; ++i)
		{
			float Angle = FMath::DegreesToRadians(BaseAngle + (i * 10.0f));

			FVector Point = TargetLocation;

			Point.X += FMath::Cos(Angle) * Radius;
			Point.Y += FMath::Sin(Angle) * Radius;

			// Point = Center에서 정확히 Radius만큼 떨어진 위치
			if (NavSystem->ProjectPointToNavigation(Point, NavLocation))
			{
				MoveLocation = NavLocation.Location;
				break;
			}
		}
	}
	else
	{
		MoveLocation = TargetLocation;
	}

	BlackboardComp->SetValueAsVector(TEXT("TargetPoint"), MoveLocation);

	return EBTNodeResult::Succeeded;
}
