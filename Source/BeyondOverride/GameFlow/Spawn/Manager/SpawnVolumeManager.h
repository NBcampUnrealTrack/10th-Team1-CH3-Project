// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "SpawnVolumeManager.generated.h"

class ASpawnVolume;

UCLASS()
class BEYONDOVERRIDE_API USpawnVolumeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	void Initialize();
	void SpawnAI();
	void CleanSpawnVolume();

  private:
	TSet<FName> ActivatedSpawnVolumes;
	TArray<TObjectPtr<ASpawnVolume>> SpawnVolumes;
};
