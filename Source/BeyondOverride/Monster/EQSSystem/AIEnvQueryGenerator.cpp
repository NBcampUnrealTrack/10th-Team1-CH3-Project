// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/AIEnvQueryGenerator.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
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
	UE_LOG(LogTemp, Warning, TEXT("AIEnvQueryGenerator Called"));
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

	FVector BaseLocation = TargetPlayer->GetActorLocation();
	const float AttackRange = MonsterAttack->GetAttackRange();
	const float PointRange = AttackRange - (AttackRange / 10);

	for (int32 i = 0; i < 72; ++i)
	{
		float RandomRange = FMath::RandRange(0.0f, AttackRange);

		float Radius = PointRange + RandomRange;

		const float Angle = FMath::DegreesToRadians(i * 5.0f);

		FVector Point = BaseLocation;

		Point.X += FMath::Cos(Angle) * Radius;
		Point.Y += FMath::Sin(Angle) * Radius;

		UE_LOG(LogTemp, Warning, TEXT("EQS Point: %s"), *Point.ToString());
		QueryInstance.AddItemData<UEnvQueryItemType_Point>(Point);
	}
}
