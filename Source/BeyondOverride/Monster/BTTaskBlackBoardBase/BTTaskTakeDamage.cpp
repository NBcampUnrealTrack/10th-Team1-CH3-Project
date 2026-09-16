// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskTakeDamage.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/MonsterCalling.h"
#include "Player/Character/BOCharacter.h"

UBTTaskTakeDamage::UBTTaskTakeDamage()
{
	NodeName = TEXT("Take Damage Check");
}

EBTNodeResult::Type UBTTaskTakeDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	bool bHitDamage = AIController->FoldFlags(EFlag::TakeDamage);
	if (!bHitDamage)
	{
		return EBTNodeResult::Succeeded;
	}

	ABOCharacter* Target = AIController->GetTarget();
	if (!Target)
	{
		return EBTNodeResult::Succeeded;
	}

	float TargetDist = FVector::Distance(Target->GetActorLocation(), AIMonster->GetActorLocation());
	float CanFind = 2000.0f;
	float CanNotFind = 5000.0f;

	UMonsterCalling* Calling = NewObject<UMonsterCalling>(AIMonster);

	if (TargetDist <= CanFind && bHitDamage)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), Target);
		Calling->CallMonsters(AIMonster->GetActorLocation(), 3000.0f, Target, ECallType::Attack);
		AIController->StateChange(EMonsterState::Chase, 10.0f);
		AIController->SetTargetPoint(FVector::ZeroVector);

		return EBTNodeResult::Succeeded;
	}

	if (TargetDist <= CanNotFind && bHitDamage)
	{
		Calling->CallMonsters(AIMonster->GetActorLocation(), 3000.0f, Target, ECallType::LocationPatrol);
	}

	return EBTNodeResult::Succeeded;
}
