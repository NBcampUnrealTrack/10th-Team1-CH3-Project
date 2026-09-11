// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/SpawnVolumeManager.h"

#include "../Spawn/SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

void USpawnVolumeManager::Initialize()
{
	ActivatedSpawnVolumes.Empty();
	SpawnVolumes.Empty();

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (TObjectPtr<ASpawnVolume> SpawnVolume = Cast<ASpawnVolume>(Actor))
		{
			SpawnVolume->OnPlayerEntered.BindUObject(this, &USpawnVolumeManager::ActivateSpawnVolume);
			SpawnVolumes.Add(SpawnVolume);
		}
	}
}

void USpawnVolumeManager::ActivateSpawnVolume(ASpawnVolume* SpawnVolume)
{
	if (ActivatedSpawnVolumes.Contains(SpawnVolume))
	{
		return;
	}

	ActivatedSpawnVolumes.Add(SpawnVolume);
	SpawnVolume->SpawnAI();
}
