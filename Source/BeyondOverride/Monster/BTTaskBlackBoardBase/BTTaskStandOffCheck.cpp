// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskStandOffCheck.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskStandOffCheck::UBTTaskStandOffCheck()
{
	NodeName = TEXT("StandOff Check");
}

EBTNodeResult::Type UBTTaskStandOffCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ABOCharacter* Target = AIController->GetTarget();
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector NowEQSPoint = BlackboardComp->GetValueAsVector(TEXT("EQSPoint"));
	if (NowEQSPoint == FVector::ZeroVector)
	{
		return EBTNodeResult::Failed;
	}

	if (AIController->GetState() == EMonsterState::StandOff)
	{
		if (AIController->IsContinueState())
		{
			BlackboardComp->SetValueAsVector(TEXT("EQSPoint"), AIMonster->GetActorLocation());
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
