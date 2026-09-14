// 26/09/13 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/EnvQueryTestNearlyAttackRange.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Player/Character/BOCharacter.h"

UEnvQueryTestNearlyAttackRange::UEnvQueryTestNearlyAttackRange()
{
	TestPurpose = EEnvTestPurpose::Score;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTestNearlyAttackRange::RunTest(FEnvQueryInstance& QueryInstance) const
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

		const FVector TargetLocation = TargetPlayer->GetActorLocation();

		const float TargetDist = FVector::Distance(TargetLocation, ItemLocation);

		const float AttackRange = MonsterAttack->GetAttackRange() / 10;

		float Score = FMath::Max(0.0f, AttackRange - FMath::Abs(TargetDist - AttackRange));

		It.SetScore(TestPurpose,
					FilterType,
					Score,
					0.0f,
					AttackRange);
	}
}
