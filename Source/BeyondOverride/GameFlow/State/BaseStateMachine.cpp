// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/BaseStateMachine.h"

#include "BaseState.h"

UBaseStateMachine::UBaseStateMachine()
	: GameMode(nullptr),
	  CurrentStageState(EStageState::None),
	  StageResult(EStageResult::None)
{
}

void UBaseStateMachine::Initialize(ABOGameMode* InGameMode)
{
	GameMode = InGameMode;
}

void UBaseStateMachine::ChangeState(EStageState StageState)
{
}

void UBaseStateMachine::SetStageResult(EStageResult Result)
{
	StageResult = Result;
}

EStageResult UBaseStateMachine::GetStageResult() const
{
	return StageResult;
}

void UBaseStateMachine::CreateState(EStageState StageState)
{
}
