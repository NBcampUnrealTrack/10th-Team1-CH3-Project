// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/EndFarmingState.h"

#include "FarmingStateMachine.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOWorldSubsystem.h"
#include "GameFlow/Manager/RegionManager.h"

void UEndFarmingState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("End Enter"));
	Super::Enter();

	SetEndTime();
	SetFarmingResult();
}

void UEndFarmingState::SetEndTime()
{
	if (!GetWorld())
	{
		return;
	}

	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Set End Time"));
		WorldSubsystem->SetEndTime();
	}
}

void UEndFarmingState::CleanRegions()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (URegionManager* RegionManager = GetWorld()->GetGameInstance()->GetSubsystem<URegionManager>())
	{
		RegionManager->CleanSetting();
	}
}

void UEndFarmingState::SetFarmingResult()
{
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
