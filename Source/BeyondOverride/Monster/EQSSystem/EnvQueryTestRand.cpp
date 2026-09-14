// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/EQSSystem/EnvQueryTestRand.h"

// Add include
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "GameFramework/Actor.h"
#include "Monster/ActorComponent/AttackDataComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/AiController/MonsterAIController.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

UEnvQueryTestRand::UEnvQueryTestRand()
{
	TestPurpose = EEnvTestPurpose::Score;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTestRand::RunTest(FEnvQueryInstance& QueryInstance) const
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
		float Score = FMath::RandRange(0.0f, 5.0f);
		It.SetScore(TestPurpose,
					FilterType,
					Score,
					0.0f,
					5.0f);
	}
}
