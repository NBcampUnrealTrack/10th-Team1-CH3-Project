// 26/09/12 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskFocusSet.h"

// Add include
#include "BehaviorTree/BlackboardComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UBTTaskFocusSet::UBTTaskFocusSet()
{
	NodeName = TEXT("Focus Set");
}

EBTNodeResult::Type UBTTaskFocusSet::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	APawn* Target = Cast<APawn>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	float RotationSpeed = 20.0f;

	FRotator TargetRotation = (Target->GetActorLocation() - AIMonster->GetActorLocation()).Rotation();

	FRotator NewRotation = FMath::RInterpTo(AIMonster->GetActorRotation(),
											TargetRotation,
											GetWorld()->GetDeltaSeconds(),
											RotationSpeed);

	AIMonster->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
