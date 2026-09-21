// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/ContinuousStateComponent.h"

#include "Monster/Enums/InfoEnums.h"
#include "Monster/Enums/StateEnums.h"

UContinuousStateComponent::UContinuousStateComponent()
{
	NowState = EMonsterState::Atmosphere;
}

void UContinuousStateComponent::OnStandOff()
{
	bContinueStandOff = true;
}

bool UContinuousStateComponent::StandOffGetPosition() const
{
	return bContinueStandOff;
}

bool UContinuousStateComponent::IsContinueState() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(StateTimer);
}

void UContinuousStateComponent::StateChange(EMonsterState Input)
{
	BeforeState = NowState;
	NowState = Input;
	OnStateCast.Broadcast(NowState);
}

void UContinuousStateComponent::StateChange(EMonsterState Input, float HoldTime)
{
	BeforeState = NowState;
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

EMonsterState UContinuousStateComponent::GetBeforeState() const
{
	return BeforeState;
}

void UContinuousStateComponent::StateAutoControl()
{

	float RangeRand = 4.0f;
	float TimerSec = 8.0f + FMath::RandRange(-RangeRand, RangeRand);

	if (NowState == EMonsterState::LocationPatrol ||
		NowState == EMonsterState::Patrol)
	{

		StateChange(EMonsterState::Atmosphere, TimerSec);
		return;
	}

	if (NowState == EMonsterState::Attack)
	{
		StateChange(EMonsterState::Chase, 10.0f);
		return;
	}

	if (NowState == EMonsterState::Chase)
	{
		if (BeforeState != EMonsterState::StandOffWait)
		{
			StateChange(EMonsterState::StandOffMove, 10.0f);
		}
		else
		{
			StateChange(EMonsterState::Atmosphere, TimerSec);
		}
		return;
	}

	if (NowState == EMonsterState::StandOffMove)
	{
		StateChange(EMonsterState::StandOffWait, 5.0f);
		bContinueStandOff = false;
		return;
	}

	if (NowState == EMonsterState::StandOffWait)
	{
		StateChange(EMonsterState::Chase, 5.0f);
		bContinueStandOff = false;
		return;
	}
}

void UContinuousStateComponent::BeginPlay()
{
	Super::BeginPlay();
}
