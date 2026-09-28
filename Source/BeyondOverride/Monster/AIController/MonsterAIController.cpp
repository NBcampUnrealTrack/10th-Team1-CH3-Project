// 26/09/09 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/AiController/MonsterAIController.h"

// Add include
#include "NavigationSystem.h"
#include "TimerManager.h"

#include "Monster/ActorComponent/AirNavComponent.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/ActorComponent/SenseComponent.h"
#include "Monster/ActorComponent/ShortTermStateComponent.h"
#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/StateEnums.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/Character/BOCharacter.h"

AMonsterAIController::AMonsterAIController()
{
	// 상태 데이터 컴포넌트
	State = CreateDefaultSubobject<UContinuousStateComponent>(TEXT("State"));
	State->OnStateCast.AddUObject(this, &AMonsterAIController::FocusSetUp);

	// 플래그 컴포넌트
	Flag = CreateDefaultSubobject<UShortTermStateComponent>(TEXT("Flag"));

	// 센서 값 컴포넌트
	SenseValue = CreateDefaultSubobject<USenseComponent>(TEXT("SenseValue"));

	// 공중 네비 컴포넌트
	AirNav = CreateDefaultSubobject<UAirNavComponent>(TEXT("AirNav"));

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
}

void AMonsterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SenseValue->SenseSetup();

	SightConfig->SightRadius = SenseValue->GetSightSenseSize();
	SightConfig->LoseSightRadius = SenseValue->GetLoseSightSize();
	SightConfig->PeripheralVisionAngleDegrees = SenseValue->GetVisionAngleDegrees();
	SightConfig->SetMaxAge(SenseValue->GetMemorize());
	AIPerception->ConfigureSense(*SightConfig);

	HearingConfig->HearingRange = SenseValue->GetHearSenseSize();
	HearingConfig->SetMaxAge(SenseValue->GetMemorize());
	AIPerception->ConfigureSense(*HearingConfig);
}

void AMonsterAIController::EnableBehaviorTree()
{
	SenseValue->SetSpawnPoint(GetMonster()->GetActorLocation());
	if (GetMonster()->GetMonsterType() != EMonsterType::Fly)
	{
		// Assets 존재 여부
		if (!BehaviorTreeAsset)
		{
			return;
		}

		// 지정 BT 에셋을 실행하는 함수
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		// Assets 존재 여부
		if (!AirBehaviorTreeAsset)
		{
			return;
		}

		// 지정 BT 에셋을 실행하는 함수
		RunBehaviorTree(AirBehaviorTreeAsset);
	}
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetHearUpdated);
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

	if (InPawn)
	{
		GetMonster()->OnStatSetComplete.AddUObject(this, &AMonsterAIController::EnableBehaviorTree);
	}
}

// 델리게이트를 통한 시야 제어
void AMonsterAIController::FocusSetUp(const EMonsterState& Input)
{
	bool Focus = true;

	if (Input == EMonsterState::Chase ||
		Input == EMonsterState::Attack)
	{
		Focus = false;
	}

	if (AMonsterCharacter* AICharacter = Cast<AMonsterCharacter>(GetPawn()))
	{
		bAllowStrafe = !Focus;
		AICharacter->FocusSetUp(Focus);
	}
}

void AMonsterAIController::MoveFlying(const FVector& TargetLocation)
{
	GetMonster()->MoveFlying(TargetLocation);
}
// 중재자 패턴용

// MonsterCharacter

UMonsterStatComponent* AMonsterAIController::GetMonsterStats() const
{
	if (!GetMonster())
	{
		return nullptr;
	}
	return GetMonster()->GetMonsterStats();
}

UMonsterDataAsset* AMonsterAIController::GetMonsterData() const
{
	if (!GetMonster())
	{
		return nullptr;
	}
	return GetMonster()->GetMonsterData();
}

AMonsterCharacter* AMonsterAIController::GetMonster() const
{
	AMonsterCharacter* MonsterCharacter = Cast<AMonsterCharacter>(GetPawn());
	if (!MonsterCharacter)
	{
		return nullptr;
	}

	return MonsterCharacter;
}

// Event

void AMonsterAIController::PlantFlag(FFlagInfo FlagInfo)
{
	Flag->PlantFlag(FlagInfo);
}

void AMonsterAIController::PlantFlag(EFlag PFlag, float Time)
{
	Flag->PlantFlag(PFlag, Time);
}

void AMonsterAIController::PlantFlag(EFlag SFlag, float Time, bool Type)
{
	Flag->PlantFlag(SFlag, Time, Type);
}

bool AMonsterAIController::FoldFlags(EFlag Target)
{
	return Flag->FoldFlags(Target);
}

bool AMonsterAIController::FoldFlags(EFlag Target, bool& Type)
{
	return Flag->FoldFlags(Target, Type);
}

// Continuous State

void AMonsterAIController::OnStandOff()
{
	State->OnStandOff();
}

bool AMonsterAIController::StandOffGetPosition() const
{
	return State->StandOffGetPosition();
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

EMonsterState AMonsterAIController::GetBeforeState() const
{
	return State->GetBeforeState();
}

bool AMonsterAIController::IsContinueState() const
{
	return State->IsContinueState();
}

// Sense

void AMonsterAIController::SetTarget(ABOCharacter* Target)
{
	SenseValue->SetTarget(Target);
}

ABOCharacter* AMonsterAIController::GetTarget() const
{
	return SenseValue->GetTarget();
}

void AMonsterAIController::SetTargetPoint(FVector Point)
{
	SenseValue->SetTargetPoint(Point);
}

FVector AMonsterAIController::GetTargetPoint() const
{
	return SenseValue->GetTargetPoint();
}

void AMonsterAIController::SetEQSPoint(FVector Point)
{
	SenseValue->SetEQSPoint(Point);
}

FVector AMonsterAIController::GetEQSPoint() const
{
	return SenseValue->GetEQSPoint();
}

FVector AMonsterAIController::GetSpawnPoint() const
{
	return SenseValue->GetSpawnPoint();
}

// AirNav

bool AMonsterAIController::AirNavControl(FVector TargetLocation)
{
	return AirNav->AirNavControl(TargetLocation);
}

FVector AMonsterAIController::AirNavResult()
{
	return AirNav->AirNavResult();
}

bool AMonsterAIController::PathControl()
{
	return AirNav->PathControl();
}
