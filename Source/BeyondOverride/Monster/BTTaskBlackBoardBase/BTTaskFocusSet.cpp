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

	FRotator TargetRotation;

	FRotator NewRotation;

	if (AIController->GetState() == EMonsterState::Chase ||
		AIController->GetState() == EMonsterState::Attack)
	{
		TargetRotation = (Target->GetActorLocation() - AIMonster->GetActorLocation()).Rotation();
		NewRotation = FMath::RInterpTo(AIMonster->GetActorRotation(),
									   TargetRotation,
									   GetWorld()->GetDeltaSeconds(),
									   RotationSpeed);
	}
	else
	{
		FVector Velocity = AIMonster->GetVelocity();

		if (!Velocity.IsNearlyZero())
		{
			TargetRotation = Velocity.GetSafeNormal().Rotation();

			NewRotation = FMath::RInterpTo(AIMonster->GetActorRotation(),
										   TargetRotation,
										   GetWorld()->GetDeltaSeconds(),
										   RotationSpeed);
		}
	}
	AIMonster->SetActorRotation(NewRotation);

	return EBTNodeResult::Succeeded;
}
