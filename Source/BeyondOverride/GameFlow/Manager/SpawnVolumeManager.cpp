// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/SpawnVolumeManager.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Spawn/SpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"

void USpawnVolumeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SpawnVolumeDatas.Empty();
	PhaseDatas.Empty();

	SpawnVolumeByRegion.Empty();

	LoadSpawnVolumeData();
	LoadPhaseData();
}

void USpawnVolumeManager::LoadSpawnVolumeData()
{
	if (!GetWorld())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("No World"));
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* BODataAsset = GameInstance->GetBODataAsset();
	if (!BODataAsset)
	{
		return;
	}

	UDataTable* SpawnVolumeDataTable = BODataAsset->GetSpawnVolumeDataTable();
	if (!SpawnVolumeDataTable)
	{
		return;
	}

	const TMap<FName, uint8*>& AllRows = SpawnVolumeDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& pair : AllRows)
	{
		FName RegionID = pair.Key;
		FSpawnVolumeData* SpawnVolumeData = reinterpret_cast<FSpawnVolumeData*>(pair.Value);

		SpawnVolumeDatas.Add(RegionID, *SpawnVolumeData);
	}
}

void USpawnVolumeManager::LoadPhaseData()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* BODataAsset = GameInstance->GetBODataAsset();
	if (!BODataAsset)
	{
		return;
	}

	UDataTable* PhaseDataTable = BODataAsset->GetPhaseDataTable();
	if (!PhaseDataTable)
	{
		return;
	}

	const TMap<FName, uint8*>& AllRows = PhaseDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& pair : AllRows)
	{
		FName RegionID = pair.Key;
		FPhaseData* PhaseData = reinterpret_cast<FPhaseData*>(pair.Value);

		PhaseDatas.Add(RegionID, *PhaseData);
	}
}

void USpawnVolumeManager::InitSetting()
{
	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (TObjectPtr<ASpawnVolume> SpawnVolume = Cast<ASpawnVolume>(Actor))
		{
			SpawnVolumeByRegion.Add(SpawnVolume->GetRegionID(), SpawnVolume);
		}
	}
}

bool USpawnVolumeManager::GetSpawnVolumeData(FName RegionID, FSpawnVolumeData& Data) const
{
	if (SpawnVolumeDatas.Contains(RegionID))
	{
		Data = SpawnVolumeDatas[RegionID];

		return true;
	}

	return false;
}

bool USpawnVolumeManager::GetPhaseData(FName RegionID, FPhaseData& Data) const
{
	if (PhaseDatas.Contains(RegionID))
	{
		Data = PhaseDatas[RegionID];

		return true;
	}

	return false;
}

ASpawnVolume* USpawnVolumeManager::GetSpawnVolume(FName RegionID) const
{
	if (SpawnVolumeByRegion.Contains(RegionID))
	{
		return SpawnVolumeByRegion[RegionID].Get();
	}

	return nullptr;
}

void USpawnVolumeManager::CleanSetting()
{
	for (TPair<FName, TWeakObjectPtr<ASpawnVolume>> Pair : SpawnVolumeByRegion)
	{
		if (Pair.Value.IsValid())
		{
			Pair.Value->CleanSetting();
		}
	}

	SpawnVolumeByRegion.Empty();
}
