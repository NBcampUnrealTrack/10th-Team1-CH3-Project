// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/ContinuousStateComponent.h"

UContinuousStateComponent::UContinuousStateComponent()
{
	NowState = EMonsterState::Atmosphere;
}

bool UContinuousStateComponent::IsContinueState() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(StateTimer);
}

void UContinuousStateComponent::StateChange(EMonsterState Input)
{
	NowState = Input;
	OnStateCast.Broadcast(NowState);
}

void UContinuousStateComponent::StateChange(EMonsterState Input, float HoldTime)
{
	NowState = Input;
	OnStateCast.Broadcast(NowState);
	GetWorld()->GetTimerManager().SetTimer(StateTimer,
										   this,
										   &UContinuousStateComponent::StateAutoControl,
										   HoldTime,
										   false);
}

EMonsterState UContinuousStateComponent::GetState() const
{
	return NowState;
}

void UContinuousStateComponent::StateAutoControl()
{
	if (NowState == EMonsterState::LocationPatrol ||
		NowState == EMonsterState::Patrol)
	{

		float RangeRand = 4.0f;
		float TimerSec = 8.0f + FMath::RandRange(-RangeRand, RangeRand);

		StateChange(EMonsterState::Atmosphere, TimerSec);
		return;
	}

	if (NowState == EMonsterState::Attack)
	{
		StateChange(EMonsterState::Chase);
		return;
	}

	if (NowState == EMonsterState::StandOff)
	{
		StateChange(EMonsterState::StandOff, 5.0f);
		StandOffPhase = 1;
		return;
	}
}

void UContinuousStateComponent::BeginPlay()
{
	Super::BeginPlay();
}
