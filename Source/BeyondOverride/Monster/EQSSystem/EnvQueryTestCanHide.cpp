// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "EnvQueryTestCanHide.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UEnvQueryTestCanHide::UEnvQueryTestCanHide()
{
	TestPurpose = EEnvTestPurpose::Score;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTestCanHide::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();

	AMonsterCharacter* QuerierMonster = Cast<AMonsterCharacter>(QuerierObject);
	if (!QuerierMonster)
	{
		return;
	}

	UStateComponent* MonsterState = QuerierMonster->GetState();
	if (!MonsterState)
	{
		return;
	}

	UAttackDataComponent* MonsterAttack = QuerierMonster->GetAttackData();
	if (!MonsterAttack)
	{
		return;
	}

	ABOCharacter* TargetPlayer = MonsterState->GetTarget();
	if (!TargetPlayer)
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		FVector StartTrace = TargetPlayer->GetActorLocation();
		FVector EndTrace = ItemLocation;

		FCollisionObjectQueryParams TraceQueryParams;
		TraceQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FCollisionQueryParams QueryParams;

		FHitResult TraceHit;

		bool isTraceHit = GetWorld()->LineTraceSingleByObjectType(TraceHit,
																  StartTrace,
																  EndTrace,
																  TraceQueryParams,
																  QueryParams);

		if (isTraceHit && TraceHit.GetActor() != TargetPlayer)
		{
			It.SetScore(TestPurpose,
						FilterType,
						1.0f,
						0.0f,
						1.0f);
		}
		else
		{
			It.SetScore(TestPurpose,
						FilterType,
						0.0f,
						0.0f,
						1.0f);
		}
	}
}
