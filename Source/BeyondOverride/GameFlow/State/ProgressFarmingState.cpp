// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/ProgressFarmingState.h"

#include "../BOGameMode.h"
#include "Player/PlayerController/BOPlayerController.h"

void UProgressFarmingState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("Progress Enter"));
	Super::Enter();

	if (!GetWorld())
	{
		return;
	}

	ShowHUDWidget();
}

void UProgressFarmingState::ShowHUDWidget()
{
	if (!GetWorld())
	{
		return;
	}

	if (ABOPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABOPlayerController>())
	{
		PlayerController->ShowMainHUDWidget();
	}
}
