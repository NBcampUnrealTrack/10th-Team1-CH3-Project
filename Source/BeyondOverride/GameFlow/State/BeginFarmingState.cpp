// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/BeginFarmingState.h"

#include "GameFlow/BOWorldSubsystem.h"
#include "GameFlow/Manager/RegionManager.h"
#include "Kismet/GameplayStatics.h"

void UBeginFarmingState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Enter"));
	Super::Enter();

	InitRegions();
	SetStartTime();

	ChangeState(EFarmingState::Progress);
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

void UBeginFarmingState::SetStartTime()
{
	if (!GetWorld())
	{
		return;
	}

	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Set Start Time"));
		WorldSubsystem->SetStartTime();
	}
}
