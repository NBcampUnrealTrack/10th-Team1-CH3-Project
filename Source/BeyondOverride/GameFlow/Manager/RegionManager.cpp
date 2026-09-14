// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/RegionManager.h"

#include "ContainerManager.h"
#include "ExitManager.h"
#include "SpawnVolumeManager.h"

#include "GameFlow/BOGameInstance.h"

void URegionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	RegionDatas.Empty();
	LoadRegionData();
}

void URegionManager::LoadRegionData()
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

	UDataTable* RegionDataTable = BODataAsset->GetRegionDataTable();
	if (!RegionDataTable)
	{
		return;
	}

	TArray<FRegionData*> AllRows{};
	RegionDataTable->GetAllRows<FRegionData>(TEXT("Get All Region Datas"), AllRows);

	for (FRegionData* Row : AllRows)
	{
		if (Row)
		{
			FName Id = Row->Id;
			RegionDatas.Add(Id, *Row);
		}
	}
}

void URegionManager::InitSetting()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	if (USpawnVolumeManager* SpawnVolumeManger = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
	{
		SpawnVolumeManger->InitSetting();
	}

	if (UContainerManager* ContainerManager = GetWorld()->GetGameInstance()->GetSubsystem<UContainerManager>())
	{
		bool IsKeyCardAcquired = GameInstance->GetIsKeyCardAcquired();
		ContainerManager->InitSetting(IsKeyCardAcquired);
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->InitSetting();
	}
}

bool URegionManager::GetRegiondata(FName RegionId, FRegionData& Data) const
{
	if (RegionDatas.Contains(RegionId))
	{
		Data = RegionDatas[RegionId];

		return true;
	}

	return false;
}
