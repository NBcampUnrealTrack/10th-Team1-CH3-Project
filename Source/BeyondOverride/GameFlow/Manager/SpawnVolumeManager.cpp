// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/SpawnVolumeManager.h"

#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Spawn/SpawnVolume.h"
#include "Kismet/GameplayStatics.h"

void USpawnVolumeManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SpawnVolumeDatas.Empty();
	PhaseDatas.Empty();

	ActivatedSpawnVolumes.Empty();
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

	TArray<FSpawnData*> AllRows{};
	SpawnVolumeDataTable->GetAllRows<FSpawnData>(TEXT("Get All Spawn Volume Datas"), AllRows);

	for (FSpawnData* Row : AllRows)
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
			SpawnVolume->OnPlayerEntered.BindUObject(this, &USpawnVolumeManager::ActivateSpawnVolume);
			SpawnVolumeByRegion.Add(SpawnVolume->GetRegionID(), SpawnVolume);
		}
	}
}

void USpawnVolumeManager::ActivateSpawnVolume(ASpawnVolume* SpawnVolume)
{
	if (ActivatedSpawnVolumes.Contains(SpawnVolume))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Activate Spawn Volume"));
	ActivatedSpawnVolumes.Add(SpawnVolume);
	SpawnVolume->SpawnMonsters();
}

bool USpawnVolumeManager::GetSpawnVolumeData(FName SpawnVolumeID, FSpawnData& Data) const
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

	ActivatedSpawnVolumes.Empty();
	SpawnVolumeByRegion.Empty();
}
