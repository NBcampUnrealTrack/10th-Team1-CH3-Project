// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskPatrolCheck.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskPatrolCheck::UBTTaskPatrolCheck()
{
	NodeName = TEXT("Patrol Check");
}

EBTNodeResult::Type UBTTaskPatrolCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (AIController->GetState() == EMonsterState::Atmosphere &&
		!AIController->IsContinueState())
	{
		AIController->StateChange(EMonsterState::Patrol, 10.0f);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
