// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Defense/BaseDefenseState.h"

#include "DefenseStateMachine.h"

void UBaseDefenseState::Initialize(UBaseStateMachine* InStateMachine)
{
	Super::Initialize(InStateMachine);

	DefenseStateMachine = Cast<UDefenseStateMachine>(InStateMachine);
}

void UBaseDefenseState::InitSetting()
{
	Super::InitSetting();
}

void UBaseDefenseState::Enter()
{
	Super::Enter();
}

void UBaseDefenseState::Exit()
{
	Super::Exit();
}

void UBaseDefenseState::ChangeState(EStageState StageState) const
{
	Super::ChangeState(StageState);

	if (DefenseStateMachine)
	{
		DefenseStateMachine->ChangeState(StageState);
	}
}
