// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/EnvQueryTestHorizontal.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UEnvQueryTestHorizontal::UEnvQueryTestHorizontal()
{
	TestPurpose = EEnvTestPurpose::Score;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTestHorizontal::RunTest(FEnvQueryInstance& QueryInstance) const
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

		bool IsHorizontal = false;

		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		const float AttackRange = MonsterAttack->GetAttackRange();
		const float PointRange = AttackRange * 2 - (AttackRange / 10);

		for (int32 i = 1; i < 3; ++i)
		{

			const float Angle = FMath::DegreesToRadians(i * 90.0f);

			FVector Point = ItemLocation;

			Point.X += FMath::Cos(Angle) * PointRange;
			Point.Y += FMath::Sin(Angle) * PointRange;

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

			const float RAngle = FMath::DegreesToRadians(i * -90.0f);

			FVector RPoint = ItemLocation;

			RPoint.X += FMath::Cos(RAngle) * PointRange;
			RPoint.Y += FMath::Sin(RAngle) * PointRange;

			FVector RStartTrace = TargetPlayer->GetActorLocation();
			FVector REndTrace = ItemLocation;

			FCollisionObjectQueryParams RTraceQueryParams;
			RTraceQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

			FCollisionQueryParams RQueryParams;

			FHitResult RTraceHit;

			bool isRTraceHit = GetWorld()->LineTraceSingleByObjectType(RTraceHit,
																	   RStartTrace,
																	   REndTrace,
																	   RTraceQueryParams,
																	   RQueryParams);

			if (isTraceHit && isRTraceHit)
			{
				IsHorizontal = true;
			}
		}

		if (IsHorizontal)
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
