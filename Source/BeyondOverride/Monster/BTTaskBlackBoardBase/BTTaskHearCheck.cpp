// 26/09/12 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskHearCheck.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskHearCheck::UBTTaskHearCheck()
{
	NodeName = TEXT("Hearing Check");
}

EBTNodeResult::Type UBTTaskHearCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UAIPerceptionComponent* Perception = AIController->GetPerceptionComponent();
	if (!Perception)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterCharacter* AIMonster = Cast<AMonsterCharacter>(AIController->GetPawn());
	if (!AIMonster)
	{
		return EBTNodeResult::Failed;
	}

	UStateComponent* AIState = AIMonster->GetState();
	if (!AIState)
	{
		return EBTNodeResult::Failed;
	}

	if (AIState->IsHearing() || AIState->IsLocation())
	{
		if (!AIState->GetBeCanPatrol())
		{
			return EBTNodeResult::Failed;
		}
		if (AIState->IsHearing())
		{
			AIState->CallLocationPatrolTimer();
		}

		AIState->FalseBeCanPatrol();
		AIState->CallPatrolTimer();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
