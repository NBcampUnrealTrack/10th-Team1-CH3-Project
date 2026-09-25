// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Farming/ProgressFarmingState.h"

#include "GameFlow/BOWorldSubsystem.h"
#include "GameFlow/Manager/ExitManager.h"
#include "Logging/BOLog.h"

void UProgressFarmingState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Farming Progress Enter"));

	Super::Enter();

	SpawnCharacter();
	ActivateExits();
	SetStartTime();
}

void UProgressFarmingState::SpawnCharacter()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->SpawnCharacter();
	}
}

void UProgressFarmingState::ActivateExits()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->ActivateExits();
	}
}

void UProgressFarmingState::SetStartTime()
{
	if (!GetWorld())
	{
		return;
	}

	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Set Start Time"));
		WorldSubsystem->SetStartTime();
	}
}
