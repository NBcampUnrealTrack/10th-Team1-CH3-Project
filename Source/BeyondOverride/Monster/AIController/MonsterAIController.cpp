// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/AiController/MonsterAIController.h"

// Add include
#include "NavigationSystem.h"
#include "TimerManager.h"

#include "Monster/ActorComponent/StateComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 50.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AMonsterAIController::EnableBehaviorTree()
{
	// Assets 존재 여부
	if (!BehaviorTreeAsset)
	{
		return;
	}

	// 지정 BT 에셋을 실행하는 함수
	RunBehaviorTree(BehaviorTreeAsset);
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	EnableBehaviorTree();
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	// 부모 클래스의 로직 상속
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
	}
}
