// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/AiController/MonsterAIController.h"

// Add include
#include "NavigationSystem.h"
#include "TimerManager.h"

#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/Character/BOCharacter.h"

AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2500.0f;
	SightConfig->LoseSightRadius = 3000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 50.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 1750.0f;
	HearingConfig->SetMaxAge(3.0f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*HearingConfig);
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
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetHearUpdated);
	EnableBehaviorTree();
}

void AMonsterAIController::OnTargetHearUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	UE_LOG(LogTemp, Warning, TEXT("Perception Updated : %s"), *GetNameSafe(Actor));
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hearing"));
	}
	else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Sight"));
	}
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hearing Success : %s"), Stimulus.WasSuccessfullySensed() ? TEXT("TRUE") : TEXT("FALSE"));
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hearing Stimulus"));

			ABOCharacter* NoiseActor = Cast<ABOCharacter>(Actor);
			AMonsterCharacter* Monster = Cast<AMonsterCharacter>(GetPawn());

			FVector NoiseLocation = Stimulus.StimulusLocation;

			if (!NoiseActor)
			{
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Heard Actor: %s"), *GetNameSafe(Actor));
			Monster->GetState()->SetLocationPatrolPoint(NoiseLocation);
			Monster->GetState()->SetLocationPatrolActor(NoiseActor);
			Monster->GetState()->CallHearingTimer();
		}
	}
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	// 부모 클래스의 로직 상속
	Super::OnPossess(InPawn);

	if (!InPawn)
	{
	}
}
