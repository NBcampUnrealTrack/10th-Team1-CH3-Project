// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskTargetPoint.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BFLCircleSerchPoint.h"

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

	APawn* Target = Cast<APawn>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	if (AIController->GetState() == EMonsterState::Attack ||
		AIController->GetState() == EMonsterState::StandOffMove ||
		AIController->GetState() == EMonsterState::StandOffWait)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	FVector TargetLocation = Target->GetActorLocation();

	FVector MoveLocation = FVector::ZeroVector;
	FVector BestLocation = FVector::ZeroVector;

	float AttackRange = AIMonster->GetAttackRange();
	float TargetDistance = FVector::Distance(TargetLocation, AIMonster->GetActorLocation());
	float BaseAngle = (AIMonster->GetActorLocation() - TargetLocation).Rotation().Yaw;

	if (NavSystem && (!NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation) || TargetDistance < AttackRange))
	{

		FSerchValues SerchData;
		SerchData.Smaple = 144;
		SerchData.Radius = AIMonster->GetAttackRange() - (AIMonster->GetAttackRange() / 10);
		SerchData.XYRange = AIMonster->GetAttackRange() / 20.0f;
		SerchData.ZRange = 2000.0f;
		SerchData.Centor = TargetLocation;
		SerchData.BaseAngle = BaseAngle;

		MoveLocation = UBFLCircleSerchPoint::CircleSerch(false, false, nullptr, SerchData, GetWorld());

		BaseAngle = (TargetLocation, MoveLocation).Rotation().Yaw;
		SerchData.Smaple = 144;
		SerchData.Radius = FVector::Distance(TargetLocation, MoveLocation);
		SerchData.XYRange = AIMonster->GetAttackRange() / 20.0f;
		SerchData.ZRange = 2000.0f;
		SerchData.Centor = TargetLocation;
		SerchData.BaseAngle = BaseAngle;

		BestLocation = UBFLCircleSerchPoint::CircleSerch(true, true, Target, SerchData, GetWorld());
	}
	else
	{
		MoveLocation = TargetLocation;
	}
	if (BestLocation != FVector::ZeroVector)
	{
		MoveLocation = BestLocation;
	}
	if (MoveLocation != FVector::ZeroVector)
	{
		BlackboardComp->SetValueAsVector(TEXT("TargetPoint"), MoveLocation);
	}

	return EBTNodeResult::Succeeded;
}
