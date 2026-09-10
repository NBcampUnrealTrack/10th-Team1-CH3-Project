// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/FarmingStateMachine.h"

#include "BeginFarmingState.h"
#include "EndFarmingState.h"
#include "NoneFarmingState.h"
#include "ProgressFarmingState.h"

UFarmingStateMachine::UFarmingStateMachine()
	: GameMode(nullptr),
	  CurrentState(nullptr),
	  CurrentFarmingState(EFarmingState::None),
	  FarmingResult(EFarmingResult::None)
{
}

void UFarmingStateMachine::Initialize(ABOGameMode* InGameMode)
{
	GameMode = InGameMode;
}

void UFarmingStateMachine::ChangeState(EFarmingState FarmingState)
{
	if (CurrentState)
	{
		CurrentState->Exit();
	}

	CurrentFarmingState = FarmingState;
	CreateState(CurrentFarmingState);

	if (CurrentState)
	{
		CurrentState->Initialize(this);
		CurrentState->Enter();
	}
}

void UFarmingStateMachine::SetFarmingResult(EFarmingResult Result)
{
	FarmingResult = Result;
}

EFarmingResult UFarmingStateMachine::GetFarmingResult() const
{
	return FarmingResult;
}

void UFarmingStateMachine::CreateState(EFarmingState FarmingState)
{
	switch (FarmingState)
	{
		case EFarmingState::None:
		{
			CurrentState = NewObject<UNoneFarmingState>();
			break;
		}
		case EFarmingState::Begin:
		{
			CurrentState = NewObject<UBeginFarmingState>();
			break;
		}
		case EFarmingState::Progress:
		{
			CurrentState = NewObject<UProgressFarmingState>();
			break;
		}
		case EFarmingState::End:
		{
			CurrentState = NewObject<UEndFarmingState>();
			break;
		}
		default:
			break;
	}
}
