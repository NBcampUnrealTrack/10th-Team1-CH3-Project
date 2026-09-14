// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskStandOffContinue.h"

// Add include
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskStandOffContinue::UBTTaskStandOffContinue()
{
	NodeName = TEXT("Continue StandOff");
}

EBTNodeResult::Type UBTTaskStandOffContinue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UStateComponent* AIState = AIMonster->GetState();
	if (!AIState)
	{
		return EBTNodeResult::Failed;
	}

	if (AIState->IsStandOff() && !AIState->IsContinueStandOff())
	{
		AIState->SetContinueStandOffTimer();
		AIState->SetStandOffTimer();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		BlackboardComp->SetValueAsVector(TEXT("TargetPoint"), AIMonster->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
}
