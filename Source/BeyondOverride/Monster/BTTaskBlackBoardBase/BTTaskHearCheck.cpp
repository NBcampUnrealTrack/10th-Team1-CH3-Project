// 26/09/12 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskHearCheck.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/ActorComponent/ShortTermStateComponent.h"
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

	bool HearCheck = AIController->FoldFlags(EFlag::Hearing);

	if (HearCheck || AIController->GetState() == EMonsterState::LocationPatrol)
	{
		if ((AIController->GetState() == EMonsterState::Chase ||
			 AIController->GetState() == EMonsterState::Attack ||
			 AIController->GetState() == EMonsterState::StandOff ||
			 AIController->GetState() == EMonsterState::Atmosphere) &&
			AIController->IsContinueState())
		{
			return EBTNodeResult::Failed;
		}
		if (HearCheck)
		{
			AIController->StateChange(EMonsterState::LocationPatrol, 10.0f);
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
