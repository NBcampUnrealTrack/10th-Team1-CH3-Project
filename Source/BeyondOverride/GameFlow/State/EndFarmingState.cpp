// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/EndFarmingState.h"

#include "FarmingStateMachine.h"

#include "../BOGameInstance.h"

void UEndFarmingState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("End Enter"));
	Super::Enter();

	if (!GetWorld())
	{
		return;
	}

	EFarmingResult FarmingResult = StateMachine->GetFarmingResult();

	if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
	{
		GameInstance->EndFarming(FarmingResult);
	}
}
