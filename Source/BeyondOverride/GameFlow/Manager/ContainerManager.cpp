// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ContainerManager.h"

#include "RegionManager.h"

#include "Algo/RandomShuffle.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/ItemDataSubsystem.h"

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

void UContainerManager::InitSetting(bool IsKeyCardAcquired)
{
	ContainerByRegion.Empty();
	bShouldSpawnKeyCard = !IsKeyCardAcquired;

	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageContainerActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		AStorageContainerActor* Container = Cast<AStorageContainerActor>(Actor);

		FName ContainerId = Container->StorageContainerId; // change to getter function
		FName RegionId = ContainerDatas[ContainerId].RegionId;

		if (!ContainerByRegion.Contains(RegionId))
		{
			ContainerByRegion.Add(RegionId);
		}

		ContainerByRegion[RegionId].Add(Container);
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

	for (const TPair<FName, TArray<TObjectPtr<AStorageContainerActor>>>& Pair : ContainerByRegion)
	{
		FName RegionId = Pair.Key;
		TArray<TObjectPtr<AStorageContainerActor>> Containers = Pair.Value;

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
			TObjectPtr<AStorageContainerActor> Container = Containers[i];

			TArray<TObjectPtr<UItemInstanceBase>> Items;
			GetSpawnItems(Container, Items);

			Container->SetItems(Items);
		}
	}
}

void UContainerManager::GetSpawnItems(AStorageContainerActor* Container, TArray<TObjectPtr<UItemInstanceBase>>& Items)
{
	if (!Container)
	{
		return;
	}

	Items.Empty();
	TMap<FName, int32> SpawnItems{};

	FSpawnData ContainerData{};
	GetContainerData(Container->StorageContainerId, ContainerData); // change to GetId()

	TArray<FSpawnEntry> SpawnEntries = ContainerData.SpawnEntries;
	int32 Count = FMath::RandRange(ContainerData.MinSpawnCount, ContainerData.MaxSpawnCount);

	for (int i = 0; i < Count; i++)
	{
		FName ItemId = AddRandomSpawnItem(SpawnEntries);

		if (SpawnItems.Contains(ItemId))
		{
			SpawnItems.Add(ItemId, 1);
		}
		else
		{
			SpawnItems[ItemId] += 1;
		}
	}

	FItemFactory ItemFactory{};

	for (const TPair<FName, int32>& Item : SpawnItems)
	{
		if (UItemInstanceBase* ItemInstanceBase = ItemFactory.CreateItemInstance(this, Item.Key, Item.Value))
		{
			Items.Add(ItemInstanceBase);
		}
	}
}

FName UContainerManager::AddRandomSpawnItem(const TArray<FSpawnEntry>& SpawnEntries)
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return FName(TEXT("Default"));
	}

	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	if (!ItemDataSubsystem)
	{
		return FName(TEXT("Default"));
	}

	float Prob = FMath::RandRange(0.0f, 100.0f);
	float Sum{};

	for (const FSpawnEntry& SpawnEntry : SpawnEntries)
	{
		Sum += SpawnEntry.Prob;

		if (Sum >= Prob)
		{
			FName ItemId = SpawnEntry.Id;
			const FItemDataRow* ItemData = ItemDataSubsystem->GetItemData(ItemId);

			if (ItemData && ItemData->DisplayName.EqualTo(FText::FromString(TEXT("KeyCard"))))
			{
				if (!bShouldSpawnKeyCard)
				{
					Sum -= SpawnEntry.Prob;

					continue;
				}
				else
				{
					bShouldSpawnKeyCard = false;
				}
			}

			return ItemId;
		}
	}

	return FName(TEXT("Default"));
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
