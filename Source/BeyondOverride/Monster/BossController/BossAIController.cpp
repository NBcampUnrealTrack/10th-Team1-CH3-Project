// 26/09/25 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BossController/BossAIController.h"

// Add include
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Monster/ActorComponent/BossPhaseComponent.h"
#include "Monster/ActorComponent/ContinuousStateComponent.h"
#include "Monster/BossCharacter/BossCharacter.h"

ABossAIController::ABossAIController()
{
	// 상태 데이터 컴포넌트
	BossPhase = CreateDefaultSubobject<UBossPhaseComponent>(TEXT("BossPhase"));
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	// 부모 클래스의 로직 상속
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		ABossCharacter* Boss = GetBoss();
		if (!Boss)
		{
			return;
		}

		Boss->HalfHealth.AddUObject(this, &ABossAIController::SetPhase2);
	}
}

void ABossAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	State->OnStateCast.AddUObject(this, &ABossAIController::StandOffBlocking);
}

void ABossAIController::SetPhase1()
{
	BossPhase->SetPhase1();
}

void ABossAIController::SetPhase2()
{
	RecallStart();
	BossPhase->SetPhase2();
}

void ABossAIController::RecallStart()
{
	bRecall = true;
	GetWorld()->GetTimerManager().SetTimer(RecallTimer,
										   this,
										   &ABossAIController::RecallEnd,
										   2.60f,
										   false);
}

void ABossAIController::RecallEnd()
{
	bRecall = false;
}

bool ABossAIController::IsRecall()
{
	return bRecall;
}

EBossPhase ABossAIController::CurrentPhase() const
{
	return BossPhase->CurrentPhase();
}

ABossCharacter* ABossAIController::GetBoss() const
{
	ABossCharacter* BossCharacter = Cast<ABossCharacter>(GetMonster());
	if (!BossCharacter)
	{
		return nullptr;
	}

	return BossCharacter;
}

void ABossAIController::StandOffBlocking(const EMonsterState& CastState)
{
	if (CastState == EMonsterState::StandOffMove ||
		CastState == EMonsterState::StandOffWait)
	{
		StateChange(EMonsterState::Chase, 10.0f);
	}
}

EBossPattern ABossAIController::PatternCast() const
{
	return Pattern;
}

void ABossAIController::PatternHold(EBossPattern HoldPattern)
{
	Pattern = HoldPattern;
}
