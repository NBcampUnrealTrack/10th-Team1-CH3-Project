// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskPerceptionCheck.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/MonsterCalling.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/Character/BOCharacter.h"

UBTTaskPerceptionCheck::UBTTaskPerceptionCheck()
{
	NodeName = TEXT("Perception Check");
}

EBTNodeResult::Type UBTTaskPerceptionCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	ABOCharacter* NearestTarget;

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

	NearestTarget = Cast<ABOCharacter>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (NearestTarget)
	{

		return EBTNodeResult::Succeeded;
	}

	PerceptionCheck(OwnerComp, NearestTarget);

	if (NearestTarget)
	{
		AIController->SetTarget(NearestTarget);
		UMonsterCalling* Calling = NewObject<UMonsterCalling>(AIMonster);
		Calling->CallMonsters(AIMonster->GetActorLocation(), 1000.0f, NearestTarget, ECallType::Attack);
		BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), NearestTarget);
		AIController->StateChange(EMonsterState::Chase, 10.0f);
		AIController->SetTargetPoint(FVector::ZeroVector);
	}

	return EBTNodeResult::Succeeded;
}

void UBTTaskPerceptionCheck::PerceptionCheck(UBehaviorTreeComponent& OwnerComp, ABOCharacter*& NearestTarget)
{
	TArray<AActor*> PerceivedActors;

	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	UAIPerceptionComponent* Perception = AIController->GetPerceptionComponent();

	if (!Perception)
	{
		return;
	}

	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(),
											PerceivedActors);

	if (PerceivedActors.IsEmpty())
	{
		return;
	}

	for (AActor* Actor : PerceivedActors)
	{
		ABOCharacter* NewTarget = Cast<ABOCharacter>(Actor);

		if (!NewTarget)
		{
			continue;
		}

		// 감지된 Character 중 최단 거리
		if (!NearestTarget)
		{
			NearestTarget = NewTarget;
			continue;
		}

		float NewTargetDistance = FVector::Distance(NewTarget->GetActorLocation(), AIController->GetPawn()->GetActorLocation());
		float OldTargetDistance = FVector::Distance(NearestTarget->GetActorLocation(), AIController->GetPawn()->GetActorLocation());

		if (NewTargetDistance < OldTargetDistance)
		{
			NearestTarget = NewTarget;
		}
	}
}
