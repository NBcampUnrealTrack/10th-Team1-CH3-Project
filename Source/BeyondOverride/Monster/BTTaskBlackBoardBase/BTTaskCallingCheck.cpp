// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskCallingCheck.h"

// Add include

#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskCallingCheck::UBTTaskCallingCheck()
{
	NodeName = TEXT("Calling Check");
}

EBTNodeResult::Type UBTTaskCallingCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		return EBTNodeResult::Succeeded;
	}

	bool FlagType;
	bool FlagCheck = AIController->FoldFlags(EFlag::Calling, FlagType);

	if (!FlagType && FlagCheck)
	{
		AIController->StateChange(EMonsterState::Chase, 10.0f);
		BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), Target);
		AIMonster->FocusSetUp(true);
		AIController->bAllowStrafe = true;
		return EBTNodeResult::Succeeded;
	}

	if (FlagType && FlagCheck)
	{
		AIController->StateChange(EMonsterState::LocationPatrol, 10.0f);
		AIController->SetTargetPoint(Target->GetActorLocation());
		AIMonster->FocusSetUp(false);
		AIController->bAllowStrafe = false;
		return EBTNodeResult::Succeeded;
	}

	AIMonster->FocusSetUp(false);
	AIController->bAllowStrafe = false;
	return EBTNodeResult::Succeeded;
}
