// Fill out your copyright notice in the Description page of Project Settings.
// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BTTaskBlackBoardBase/BTTaskOverlapCheck.h"

// Add include
#include "CollisionQueryParams.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/MonsterCalling.h"
#include "Player/Character/BOCharacter.h"

UBTTaskOverlapCheck::UBTTaskOverlapCheck()
{
	NodeName = TEXT("Oveplap Check");
}

EBTNodeResult::Type UBTTaskOverlapCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	UStateComponent* AIState = AIMonster->GetState();
	if (!AIState)
	{
		return EBTNodeResult::Failed;
	}

	NearestTarget = Cast<ABOCharacter>(BlackboardComp->GetValueAsObject(TEXT("TargetPlayer")));
	if (NearestTarget)
	{

		return EBTNodeResult::Succeeded;
	}

	OverlapAndTraceCheck(OwnerComp, NearestTarget);

	if (NearestTarget)
	{
		UMonsterCalling* Calling = NewObject<UMonsterCalling>(AIMonster);
		Calling->CallMonsters(AIMonster->GetActorLocation(), 1000.0f, NearestTarget, ECallType::Attack);
		BlackboardComp->SetValueAsObject(TEXT("TargetPlayer"), NearestTarget);
		AIState->TrueContinueTargeting();
		AIState->CallContinueTimer();
		AIState->SetSttandOffTimer();
	}

	return EBTNodeResult::Succeeded;
}

void UBTTaskOverlapCheck::OverlapAndTraceCheck(UBehaviorTreeComponent& OwnerComp, ABOCharacter*& NearestTarget)
{
	AMonsterAIController* AIController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	// 결과 값 저장용 배열
	TArray<FOverlapResult> OverlapResults;

	// Query 설정용 구조체
	FCollisionObjectQueryParams ObjectQueryParams;

	// 인식할 Actor 설정 ECC_Pawn
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape =
		FCollisionShape::MakeSphere(800.0f);

	bool bHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,
													 AIController->GetPawn()->GetActorLocation(),
													 FQuat::Identity,
													 ObjectQueryParams,
													 CollisionShape);

	if (OverlapResults.IsEmpty())
	{
		return;
	}

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor)
		{
			continue;
		}

		ABOCharacter* NewTarget = Cast<ABOCharacter>(Actor);

		if (!NewTarget)
		{
			continue;
		}

		FVector StartTrace = NewTarget->GetActorLocation();
		FVector EndTrace = AIController->GetPawn()->GetActorLocation();

		FCollisionObjectQueryParams TraceQueryParams;
		TraceQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		TraceQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(NewTarget);

		FHitResult TraceHit;

		bool isTraceHit = GetWorld()->LineTraceSingleByObjectType(TraceHit,
																  StartTrace,
																  EndTrace,
																  TraceQueryParams,
																  QueryParams);

		if (!isTraceHit)
		{
			continue;
		}
		if (isTraceHit && TraceHit.GetActor() != AIController->GetPawn())
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
