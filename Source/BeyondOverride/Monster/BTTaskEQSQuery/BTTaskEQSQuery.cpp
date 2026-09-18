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

	if (AIController->GetState() == EMonsterState::StandOffMove &&
		BlackboardComp->IsVectorValueSet(TEXT("EQSPoint")))
	{
		return EBTNodeResult::Failed;
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
