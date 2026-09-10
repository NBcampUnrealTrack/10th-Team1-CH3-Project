// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/BaseFarmingState.h"

#include "FarmingStateMachine.h"

void UBaseFarmingState::Initialize(UFarmingStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
}

void UBaseFarmingState::Enter()
{
}

void UBaseFarmingState::Exit()
{
}

void UBaseFarmingState::ChangeState(EFarmingState FarmingState) const
{
	if (StateMachine)
	{
		StateMachine->ChangeState(FarmingState);
	}
}
