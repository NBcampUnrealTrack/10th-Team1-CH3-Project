// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/AiController/MonsterAIController.h"

// Add include
#include "NavigationSystem.h"
#include "TimerManager.h"

#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/ActorComponent/SenseComponent.h"
#include "Monster/ActorComponent/ShortTermStateComponent.h"
#include "Monster/ActorComponent/StateComponent.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/Character/BOCharacter.h"

AMonsterAIController::AMonsterAIController()
{
	// 상태 데이터 컴포넌트
	State = CreateDefaultSubobject<UContinuousStateComponent>(TEXT("State"));

	// 플래그 컴포넌트
	Flag = CreateDefaultSubobject<UShortTermStateComponent>(TEXT("Flag"));

	// 센서 값 컴포넌트
	SenseValue = CreateDefaultSubobject<USenseComponent>(TEXT("SenseValue"));

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
	HearingConfig->SetMaxAge(5.0f);

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
	SenseValue->SetSpawnPoint(GetPawn()->GetActorLocation());
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetHearUpdated);
	EnableBehaviorTree();
}

void AMonsterAIController::OnTargetHearUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{

			ABOCharacter* NoiseActor = Cast<ABOCharacter>(Actor);
			AMonsterCharacter* Monster = Cast<AMonsterCharacter>(GetPawn());

			FVector NoiseLocation = Stimulus.StimulusLocation;

			if (!NoiseActor)
			{
				return;
			}

			PlantFlag(EFlag::Hearing, CurrentTime);
			SetTargetPoint(NoiseLocation);
			SetTarget(NoiseActor);
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

// 중재자 패턴용

void AMonsterAIController::PlantFlag(FFlagInfo FlagInfo)
{
	Flag->PlantFlag(FlagInfo);
}

void AMonsterAIController::PlantFlag(EFlag PFlag, float Time)
{
	Flag->PlantFlag(PFlag, Time);
}

bool AMonsterAIController::FoldFlags(EFlag Target)
{
	return Flag->FoldFlags(Target);
}

void AMonsterAIController::StateChange(EMonsterState Input)
{
	State->StateChange(Input);
}

void AMonsterAIController::StateChange(EMonsterState Input, float HoldTime)
{
	State->StateChange(Input, HoldTime);
}

EMonsterState AMonsterAIController::GetState() const
{

	return State->GetState();
}

bool AMonsterAIController::IsContinueState() const
{
	return State->IsContinueState();
}

void AMonsterAIController::SetTarget(ABOCharacter* Target)
{
	SenseValue->SetTarget(Target);
}

void AMonsterAIController::SetTargetPoint(FVector Point)
{
	SenseValue->SetTargetPoint(Point);
}

ABOCharacter* AMonsterAIController::GetTarget() const
{
	return SenseValue->GetTarget();
}

FVector AMonsterAIController::GetTargetPoint() const
{
	return SenseValue->GetTargetPoint();
}
