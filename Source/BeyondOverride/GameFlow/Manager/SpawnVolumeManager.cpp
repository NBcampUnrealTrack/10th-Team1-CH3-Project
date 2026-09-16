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
			FName Id = Row->Id;
			SpawnVolumeDatas.Add(Id, *Row);
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
			FName SpawnVolumeId = Row->SpawnVolumeId;
			PhaseDatas.Add(SpawnVolumeId, *Row);
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
			SpawnVolumeByRegion.Add(SpawnVolume->GetRegionId(), SpawnVolume);
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

bool USpawnVolumeManager::GetSpawnVolumeData(FName SpawnVolumeId, FSpawnData& Data) const
{
	if (SpawnVolumeDatas.Contains(SpawnVolumeId))
	{
		Data = SpawnVolumeDatas[SpawnVolumeId];

		return true;
	}

	return false;
}

bool USpawnVolumeManager::GetPhaseData(FName SpawnVolumeId, FPhaseData& Data) const
{
	if (PhaseDatas.Contains(SpawnVolumeId))
	{
		Data = PhaseDatas[SpawnVolumeId];

		return true;
	}

	return false;
}

ASpawnVolume* USpawnVolumeManager::GetSpawnVolume(FName RegionId) const
{
	if (SpawnVolumeByRegion.Contains(RegionId))
	{
		return SpawnVolumeByRegion[RegionId];
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
