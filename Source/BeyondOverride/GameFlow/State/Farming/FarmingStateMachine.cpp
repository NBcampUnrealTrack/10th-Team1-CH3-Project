// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Farming/FarmingStateMachine.h"

#include "BeginFarmingState.h"
#include "EndFarmingState.h"
#include "ProgressFarmingState.h"

#include "Logging/BOLog.h"

UFarmingStateMachine::UFarmingStateMachine()
	: CurrentFarmingState(nullptr)
{
}

void UFarmingStateMachine::ChangeState(EStageState StageState)
{
	Super::ChangeState(StageState);

	CurrentStageState = StageState;
	CreateState(CurrentStageState);

	if (CurrentFarmingState)
	{
		CurrentFarmingState->Initialize(this);
		CurrentFarmingState->Enter();
	}
}

void UFarmingStateMachine::CreateState(EStageState StageState)
{
	Super::CreateState(StageState);

	switch (StageState)
	{
	case EStageState::Begin:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Begin"));
		CurrentFarmingState = NewObject<UBeginFarmingState>(this, UBeginFarmingState::StaticClass());
		break;
	}
	case EStageState::Progress:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Progress"));
		CurrentFarmingState = NewObject<UProgressFarmingState>(this, UProgressFarmingState::StaticClass());
		break;
	}
	case EStageState::End:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State End"));
		CurrentFarmingState = NewObject<UEndFarmingState>(this, UEndFarmingState::StaticClass());
		break;
	}
	default:
		break;
	}
}
