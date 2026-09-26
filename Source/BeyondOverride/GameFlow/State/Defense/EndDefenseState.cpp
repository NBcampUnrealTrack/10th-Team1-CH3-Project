// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Defense/EndDefenseState.h"

#include "DefenseStateMachine.h"

#include "Logging/BOLog.h"

void UEndDefenseState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Defense End Enter"));
	Super::Enter();

	if (DefenseStateMachine)
	{
		DefenseStateMachine->OnPhaseEnded();
	}
}
