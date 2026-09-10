// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/EndFarmingState.h"

#include "FarmingStateMachine.h"

#include "../BOGameInstance.h"

void UEndFarmingState::Enter()
{
	EFarmingResult FarmingResult = StateMachine->GetFarmingResult();

	if (GetWorld())
	{
		if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
		{
			GameInstance->EndFarming(FarmingResult);
		}
	}
}
