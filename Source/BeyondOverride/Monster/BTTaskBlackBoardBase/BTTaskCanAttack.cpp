// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskCanAttack.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskCanAttack::UBTTaskCanAttack()
{
	NodeName = TEXT("Can Attack Check");
}

EBTNodeResult::Type UBTTaskCanAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ABOCharacter* Target = Cast<ABOCharacter>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	AMonsterCharacter* AIMonster = Cast<AMonsterCharacter>(BlackboardComp->GetValueAsObject(TEXT("SelfActor")));
	if (!AIMonster)
	{
		return EBTNodeResult::Failed;
	}

	float AttackRange = AIMonster->GetAttackRange();
	float TargetDistance = FVector::Distance(Target->GetActorLocation(), AIMonster->GetActorLocation());

	BlackboardComp->SetValueAsBool(TEXT("bCanAttack"), TargetDistance < AttackRange && !AIMonster->IsDelay());
	return EBTNodeResult::Succeeded;
}
