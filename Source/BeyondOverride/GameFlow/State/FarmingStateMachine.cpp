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
		case EFarmingState::Begin:
		{
			UE_LOG(LogTemp, Warning, TEXT("State Begin"));
			CurrentState = NewObject<UBeginFarmingState>(this, UBeginFarmingState::StaticClass());
			break;
		}
		case EFarmingState::Progress:
		{
			UE_LOG(LogTemp, Warning, TEXT("State Progress"));
			CurrentState = NewObject<UProgressFarmingState>(this, UProgressFarmingState::StaticClass());
			break;
		}
		case EFarmingState::End:
		{
			UE_LOG(LogTemp, Warning, TEXT("State End"));
			CurrentState = NewObject<UEndFarmingState>(this, UEndFarmingState::StaticClass());
			break;
		}
		default:
			break;
	}
}
