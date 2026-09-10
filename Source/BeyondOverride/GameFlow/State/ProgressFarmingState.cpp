// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/ProgressFarmingState.h"

#include "../Player/PlayerController/BOPlayerController.h"

void UProgressFarmingState::Enter()
{
	ShowHUDWidget();
}

void UProgressFarmingState::ShowHUDWidget()
{
	if (ABOPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABOPlayerController>())
	{
		PlayerController->ShowMainHUDWidget();
	}
}
