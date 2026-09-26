// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Farming/BaseFarmingState.h"

#include "FarmingStateMachine.h"

void UBaseFarmingState::Initialize(UBaseStateMachine* InStateMachine)
{
	Super::Initialize(InStateMachine);

	FarmingStateMachine = Cast<UFarmingStateMachine>(InStateMachine);
}

void UBaseFarmingState::Enter()
{
	Super::Enter();
}

void UBaseFarmingState::Exit()
{
	Super::Exit();
}

void UBaseFarmingState::ChangeState(EStageState StageState) const
{
	Super::ChangeState(StageState);

	if (FarmingStateMachine)
	{
		FarmingStateMachine->ChangeState(StageState);
	}
}
