// 26/09/28 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLMonsterClear.h"

// Add include
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

void UBFLMonsterClear::MonsterClear(FVector Location, float Radius, UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!World)
	{
		return;
	}

	TSet<AActor*> CompleteTargets = {};

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;

	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

	bool bHit = World->OverlapMultiByObjectType(OverlapResults,
												Location,
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

		if (CompleteTargets.Contains(Actor))
		{
			continue;
		}

		AMonsterCharacter* Target = Cast<AMonsterCharacter>(Actor);
		if (!Target)
		{
			continue;
		}

		Target->DeathSequence();
		CompleteTargets.Add(Actor);
	}
}
