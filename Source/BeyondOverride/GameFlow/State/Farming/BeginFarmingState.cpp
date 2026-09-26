// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Farming/BeginFarmingState.h"

#include "GameFlow/Manager/RegionManager.h"
#include "Logging/BOLog.h"

void UBeginFarmingState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Farming Begin Enter"));
	Super::Enter();

	InitRegions();

	ChangeState(EStageState::Progress);
}

void UBeginFarmingState::InitRegions()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (URegionManager* RegionManager = GetWorld()->GetGameInstance()->GetSubsystem<URegionManager>())
	{
		RegionManager->InitSetting();
	}
}
