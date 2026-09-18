// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskStandOffContinue.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskStandOffContinue::UBTTaskStandOffContinue()
{
	NodeName = TEXT("Continue StandOff");
}

EBTNodeResult::Type UBTTaskStandOffContinue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UCharacterMovementComponent* Movement = AIMonster->GetCharacterMovement();
	if (!Movement)
	{
		return EBTNodeResult::Failed;
	}

	FVector NowEQSPoint = BlackboardComp->GetValueAsVector(TEXT("EQSPoint"));

	if (AIController->GetState() == EMonsterState::StandOffMove &&
		FVector::PointsAreNear(NowEQSPoint, AIMonster->GetActorLocation(), 100.0f))
	{
		AIController->StateChange(EMonsterState::StandOffWait, 5.0f);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
