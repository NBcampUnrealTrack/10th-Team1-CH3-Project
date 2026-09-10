// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Spawn/SpawnVolume.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnVolume::Initialize()
{
}

void ASpawnVolume::SpawnRandomAI()
{
}

APawn* ASpawnVolume::SpawnAI()
{
	return nullptr;
}

void ASpawnVolume::RemoveSpawnedAIs()
{
}
