// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/ProgressFarmingState.h"

#include "Logging/BOLog.h"

void UProgressFarmingState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Progress Enter"));

	Super::Enter();
}
