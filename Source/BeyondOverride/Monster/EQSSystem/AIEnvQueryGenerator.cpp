// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/AIEnvQueryGenerator.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UAIEnvQueryGenerator::UAIEnvQueryGenerator(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemType = UEnvQueryItemType_Point::StaticClass();
}

void UAIEnvQueryGenerator::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();

	AMonsterCharacter* QuerierMonster = Cast<AMonsterCharacter>(QuerierObject);
	if (!QuerierMonster)
	{
		return;
	}

	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(QuerierMonster->GetController());
	if (!MonsterController)
	{
		return;
	}

	ABOCharacter* TargetPlayer = MonsterController->GetTarget();
	if (!TargetPlayer)
	{
		return;
	}

	FVector BaseLocation = TargetPlayer->GetActorLocation();
	const float AttackRange = QuerierMonster->GetAttackRange();
	const float PointRange = AttackRange - (AttackRange / 10);

	for (int32 i = 0; i < 144; ++i)
	{
		float RandomRange = FMath::RandRange(0.0f, AttackRange);

		float Radius = PointRange + RandomRange;

		const float Angle = FMath::DegreesToRadians(i * 2.5f);

		FVector Point = BaseLocation;

		Point.X += FMath::Cos(Angle) * Radius;
		Point.Y += FMath::Sin(Angle) * Radius;

		QueryInstance.AddItemData<UEnvQueryItemType_Point>(Point);
	}
}
