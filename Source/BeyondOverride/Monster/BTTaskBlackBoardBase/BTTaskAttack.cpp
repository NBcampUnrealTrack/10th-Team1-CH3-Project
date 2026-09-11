// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskAttack.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UBTTaskAttack::UBTTaskAttack()
{
	NodeName = TEXT("To Attack");
}

EBTNodeResult::Type UBTTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UAttackDataComponent* AIAttackData = AIMonster->GetAttackDataComponent();
	if (!AIAttackData)
	{
		return EBTNodeResult::Failed;
	}

	UStateComponent* AIState = AIMonster->GetStateComponent();
	if (!AIState)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Target = Cast<APawn>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	AIAttackData->SetTargetLocation(Target->GetActorLocation());

	AIMonster->MonsterAttack();
	AIState->ReCallContinueTimer();
	AIAttackData->CallAttackDelay();

	return EBTNodeResult::Succeeded;
}
