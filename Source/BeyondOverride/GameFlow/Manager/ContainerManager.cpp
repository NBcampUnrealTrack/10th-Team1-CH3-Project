// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ContainerManager.h"

#include "RegionManager.h"

#include "../BOGameInstance.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

void UContainerManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ContainerDatas.Empty();

	LoadContainerData();
}

void UContainerManager::LoadContainerData()
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

	UDataTable* ContainerDataTable = BODataAsset->GetContainerDataTable();
	if (!ContainerDataTable)
	{
		return;
	}

	TArray<FSpawnData*> AllRows{};
	ContainerDataTable->GetAllRows<FSpawnData>(TEXT("Get All Container Datas"), AllRows);

	for (FSpawnData* Row : AllRows)
	{
		if (Row)
		{
			FName Id = Row->Id;
			ContainerDatas.Add(Id, *Row);
		}
	}
}

void UContainerManager::InitSetting()
{
	ContainerByRegion.Empty();

	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);  // change AActor -> AContainer
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Container"), AllActors); // test code

	for (AActor* Actor : AllActors)
	{
		// cast to Container
		// AContainer* Container = Cast<AContainer>(Actor);

		// save container by region
		/*FName RegionId = Container->GetRegionId();
		if (ContainerByRegion.Contains(RegionId))
		{
			ContainerByRegion[RegionId].Add(Actor);
		}
		else
		{
			ContainerByRegion.Add(RegionId, Actor);
		}*/
	}

	ActivateContainer();
}

void UContainerManager::ActivateContainer()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	URegionManager* RegionManager = GetWorld()->GetGameInstance()->GetSubsystem<URegionManager>();
	if (!RegionManager)
	{
		return;
	}

	bool IsKeyCardSpawned{};

	for (const TPair<FName, TArray<TObjectPtr<AActor>>>& Pair : ContainerByRegion) // change AActor -> AContainer
	{
		FName RegionId = Pair.Key;
		TArray<TObjectPtr<AActor>> Containers = Pair.Value;

		FRegionData RegionData{};
		if (!RegionManager->GetRegiondata(RegionId, RegionData))
		{
			return;
		}

		float Prob = RegionData.ContainerActivateProb;
		int32 Size = Containers.Num();
		int32 Count = FMath::RoundToInt(Size * Prob);

		Algo::RandomShuffle(Containers);

		for (int i = 0; i < Count; i++)
		{
			TObjectPtr<AActor> Container = Containers[i];

			/*
			if (IsKeyCardSpawned)
			{
				Container->SetCanSpawnKeyCard(false);
			}
			else
			{
				Container->SetCanSpawnKeyCard(true);
				IsKeyCardSpawned = true;
			}

			Container->SetSpawnItems(); // set container 'TArray<FName> Items' property
			*/
		}
	}
}

bool UContainerManager::GetContainerData(FName ContainerId, FSpawnData& Data) const
{
	if (ContainerDatas.Contains(ContainerId))
	{
		Data = ContainerDatas[ContainerId];

		return true;
	}

	return false;
}
