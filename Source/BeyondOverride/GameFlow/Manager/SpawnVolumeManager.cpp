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

	TArray<FSpawnVolumeData*> AllRows{};
	SpawnVolumeDataTable->GetAllRows<FSpawnVolumeData>(TEXT("Get All Spawn Volume Datas"), AllRows);

	for (FSpawnVolumeData* Row : AllRows)
	{
		if (Row)
		{
			FName ID = Row->ID;
			SpawnVolumeDatas.Add(ID, *Row);
		}
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

	TArray<FPhaseData*> AllRows{};
	PhaseDataTable->GetAllRows<FPhaseData>(TEXT("Get All Phase Datas"), AllRows);

	for (FPhaseData* Row : AllRows)
	{
		if (Row)
		{
			FName SpawnVolumeID = Row->SpawnVolumeID;
			PhaseDatas.Add(SpawnVolumeID, *Row);
		}
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

bool USpawnVolumeManager::GetSpawnVolumeData(FName SpawnVolumeID, FSpawnVolumeData& Data) const
{
	if (SpawnVolumeDatas.Contains(SpawnVolumeID))
	{
		Data = SpawnVolumeDatas[SpawnVolumeID];

		return true;
	}

	return false;
}

bool USpawnVolumeManager::GetPhaseData(FName SpawnVolumeID, FPhaseData& Data) const
{
	if (PhaseDatas.Contains(SpawnVolumeID))
	{
		Data = PhaseDatas[SpawnVolumeID];

		return true;
	}

	return false;
}

ASpawnVolume* USpawnVolumeManager::GetSpawnVolume(FName RegionID) const
{
	if (SpawnVolumeByRegion.Contains(RegionID))
	{
		return SpawnVolumeByRegion[RegionID];
	}

	return nullptr;
}

void USpawnVolumeManager::CleanSetting()
{
	for (TPair<FName, TObjectPtr<ASpawnVolume>> Pair : SpawnVolumeByRegion)
	{
		if (IsValid(Pair.Value))
		{
			Pair.Value->CleanSetting();
		}
	}

	SpawnVolumeByRegion.Empty();
}
