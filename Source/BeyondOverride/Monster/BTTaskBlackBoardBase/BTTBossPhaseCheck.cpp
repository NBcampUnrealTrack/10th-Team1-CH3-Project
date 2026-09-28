// 26/09/27 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTBossPhaseCheck.h"

// Add include
#include "BehaviorTree/BehaviorTree.h"
#include "Monster/BossCharacter/BossCharacter.h"
#include "Monster/BossController/BossAIController.h"
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Structs/SystemParams.h"

UBTTBossPhaseCheck::UBTTBossPhaseCheck()
{
	NodeName = TEXT("PhaseCheck");
}

EBTNodeResult::Type UBTTBossPhaseCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	ABossAIController* BossController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!BossController)
	{
		return EBTNodeResult::Failed;
	}
	if (BossController->CurrentPhase() == EBossPhase::Phase1)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsPhase2"), false);
	}
	else
	{
		BlackboardComp->SetValueAsBool(TEXT("IsPhase2"), true);
	}
	return EBTNodeResult::Succeeded;
}
