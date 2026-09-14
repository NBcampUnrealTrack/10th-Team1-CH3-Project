// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/EnvQueryTestCover.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UEnvQueryTestCover::UEnvQueryTestCover()
{
	TestPurpose = EEnvTestPurpose::Filter;
	FilterType = EEnvTestFilterType::Minimum;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTestCover::RunTest(FEnvQueryInstance& QueryInstance) const
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

		int32 HitNums = 0;
		float Score = 0;

		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		const float AttackRange = MonsterAttack->GetAttackRange();
		const float PointRange = AttackRange * 2 - (AttackRange / 10);

		for (int32 i = 0; i < 4; ++i)
		{

			const float Angle = FMath::DegreesToRadians(i * 90.0f);

			FVector Point = ItemLocation;

			Point.X += FMath::Cos(Angle) * PointRange;
			Point.Y += FMath::Sin(Angle) * PointRange;

			FVector StartTrace = TargetPlayer->GetActorLocation();
			FVector EndTrace = Point;

			FCollisionObjectQueryParams TraceQueryParams;
			TraceQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FCollisionQueryParams QueryParams;

			FHitResult TraceHit;

			bool isTraceHit = GetWorld()->LineTraceSingleByObjectType(TraceHit,
																	  StartTrace,
																	  EndTrace,
																	  TraceQueryParams,
																	  QueryParams);

			if (isTraceHit)
			{
				HitNums = HitNums + 1;
			}
		}
		switch (HitNums)
		{
		case 1:
			Score = 3;
			break;
		case 2:
			Score = 4;
			break;
		case 3:
			Score = 2;
			break;
		case 4:
			Score = 1;
			break;
		default:
			Score = 1;
			break;
		}
		It.SetScore(TestPurpose,
					FilterType,
					Score,
					0.0f,
					4.0f);
	}
}
