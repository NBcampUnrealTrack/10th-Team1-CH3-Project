// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/MonsterCalling.h"

// Add include
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

void UMonsterCalling::CallMonsters(const FVector& CallCenter, float Radius, ABOCharacter*& Target, ECallType Type)
{

	// 결과 값 저장용 배열
	TArray<FOverlapResult> OverlapResults;

	// Query 설정용 구조체
	FCollisionObjectQueryParams ObjectQueryParams;

	// 인식할 Actor 설정 ECC_Pawn
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

	bool bHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,
													 CallCenter,
													 FQuat::Identity,
													 ObjectQueryParams,
													 CollisionShape);

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();

		if (!Actor)
		{
			continue;
		}

		AMonsterCharacter* CallTarget = Cast<AMonsterCharacter>(Actor);

		if (!CallTarget)
		{
			continue;
		}
		if (!CallTarget->GetState()->GetContinueTargeting())
		{
			continue;
		}

		CallTarget->GetState()->SetCallingTimer();
		CallTarget->GetState()->SetTarget(Target);

		if (Type == ECallType::LocationPatrol)
		{
			CallTarget->GetState()->SetIsCallLocation(true);
		}
	}
}
