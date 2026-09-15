// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskClearTarget.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskClearTarget::UBTTaskClearTarget()
{
	NodeName = TEXT("Continue Target Check");
}

EBTNodeResult::Type UBTTaskClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (!AIController->IsContinueState() &&
		(AIController->GetState() == EMonsterState::Chase ||
		 AIController->GetState() == EMonsterState::Attack ||
		 AIController->GetState() == EMonsterState::StandOff))
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), nullptr);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
