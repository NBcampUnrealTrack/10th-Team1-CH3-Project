// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/ProgressFarmingState.h"

void UProgressFarmingState::Enter()
{
	UE_LOG(LogTemp, Warning, TEXT("Progress Enter"));

	Super::Enter();
}
