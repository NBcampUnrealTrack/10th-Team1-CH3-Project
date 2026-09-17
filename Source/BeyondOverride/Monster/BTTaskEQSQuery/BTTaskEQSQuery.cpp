// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskEQSQuery/BTTaskEQSQuery.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskEQSQuery::UBTTaskEQSQuery()
{
	NodeName = TEXT("Find Standoff Position");
}

EBTNodeResult::Type UBTTaskEQSQuery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
