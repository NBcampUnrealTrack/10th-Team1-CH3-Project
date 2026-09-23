// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Farming/PhaseData.h"
#include "DataTables/Farming/SpawnData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SpawnVolumeManager.generated.h"

class ASpawnVolume;

UCLASS()
class BEYONDOVERRIDE_API USpawnVolumeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadSpawnVolumeData();
	void LoadPhaseData();

  public:
	void InitSetting();

	bool GetSpawnVolumeData(FName SpawnVolumeID, FSpawnVolumeData& Data) const;
	bool GetPhaseData(FName SpawnVolumeID, FPhaseData& Data) const;
	ASpawnVolume* GetSpawnVolume(FName RegionID) const;

	void CleanSetting();

  private:
	TMap<FName, FSpawnVolumeData> SpawnVolumeDatas;
	TMap<FName, FPhaseData> PhaseDatas;

	TMap<FName, TObjectPtr<ASpawnVolume>> SpawnVolumeByRegion;
};
