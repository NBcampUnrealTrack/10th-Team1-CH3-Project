// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameMode.h"

#include "BOEnums.h"
#include "BOGameInstance.h"

#include "State/FarmingStateMachine.h"

ABOGameMode::ABOGameMode()
	: StateMachine(nullptr),
	  IsFailedFarming(false)
{
}

void ABOGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UBOGameInstance* GameInstance = GetGameInstance<UBOGameInstance>())
	{
		EGameState BOGameState = GameInstance->GetGameState();
		EPlayingState BOPlayingState = GameInstance->GetPlayingState();

		if (BOGameState == EGameState::Playing && BOPlayingState == EPlayingState::Farming)
		{
			StartFarming();
		}
	}
}

void ABOGameMode::StartFarming()
{
	StateMachine = NewObject<UFarmingStateMachine>();

	if (StateMachine)
	{
		StateMachine->Initialize(this);
		StateMachine->ChangeState(EFarmingState::Begin);
	}
}

void ABOGameMode::EndFarming(EFarmingResult Result)
{
	if (StateMachine)
	{
		StateMachine->SetFarmingResult(Result);
		StateMachine->ChangeState(EFarmingState::End);
	}
}
