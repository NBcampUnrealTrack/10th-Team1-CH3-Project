// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskEQSQuery/BTTaskEQSQuery.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"

UBTTaskEQSQuery::UBTTaskEQSQuery()
{
	NodeName = TEXT("Find Standoff Position");
}

EBTNodeResult::Type UBTTaskEQSQuery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AIController->OnStandOff();
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
