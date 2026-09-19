// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ContainerManager.h"

#include "RegionManager.h"

#include "Algo/RandomShuffle.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "Subsystems/ItemDataSubsystem.h"

void UContainerManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ContainerDatas.Empty();
	bShouldSpawnKeyCard = false;

	if (GetWorld())
	{
		GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	}

	LoadContainerData();
}

void UContainerManager::LoadContainerData()
{
	if (!GetWorld())
	{
		return;
	}

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
			FName ID = Row->ID;
			ContainerDatas.Add(ID, *Row);
		}
	}
}

void UContainerManager::InitSetting()
{
	ContainerByRegion.Empty();
	bShouldSpawnKeyCard = !IsKeyCardAcquired();

	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageContainerActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		AStorageContainerActor* Container = Cast<AStorageContainerActor>(Actor);

		FName ContainerID = Container->GetStorageContainerID();

		if (ContainerDatas.Contains(ContainerID))
		{
			FName RegionID = ContainerDatas[ContainerID].RegionID;

			if (!ContainerByRegion.Contains(RegionID))
			{
				ContainerByRegion.Add(RegionID);
			}

			ContainerByRegion[RegionID].Add(Container);
		}
	}

	ActivateContainer();
}

bool UContainerManager::IsKeyCardAcquired() const
{
	return HasPlayerKeyCard() || HasStorageKeyCard();
}

bool UContainerManager::HasPlayerKeyCard() const
{
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return false;
	}

	if (!GameInstance)
	{
		return false;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return false;
	}

	FName KeyCardID = DataAsset->GetKeyCardID();

	if (ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>())
	{
		if (UPlayerInventoryComponent* InventoryComponent = Character->GetPlayerInventoryComponent())
		{
			if (InventoryComponent->FindItemIndex(KeyCardID) != INDEX_NONE)
			{
				return true;
			}
		}
	}

	return false;
}

bool UContainerManager::HasStorageKeyCard() const
{
	if (!GameInstance)
	{
		return false;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return false;
	}

	FName KeyCardID = DataAsset->GetKeyCardID();
	TArray<UItemInstanceBase*> StorageInventory = GameInstance->GetStorageInventory();

	for (UItemInstanceBase* Item : StorageInventory)
	{
		if (Item->GetItemID() == KeyCardID)
		{
			return true;
		}
	}

	return false;
}

void UContainerManager::ActivateContainer()
{
	if (!GameInstance)
	{
		return;
	}

	URegionManager* RegionManager = GameInstance->GetSubsystem<URegionManager>();
	if (!RegionManager)
	{
		return;
	}

	for (const TPair<FName, TArray<TObjectPtr<AStorageContainerActor>>>& Pair : ContainerByRegion)
	{
		FName RegionID = Pair.Key;
		TArray<TObjectPtr<AStorageContainerActor>> Containers = Pair.Value;

		FRegionData RegionData{};
		if (!RegionManager->GetRegiondata(RegionID, RegionData))
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

			TArray<TObjectPtr<UItemInstanceBase>> Items{};
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
	if (!GetContainerData(Container->GetStorageContainerID(), ContainerData))
	{
		return;
	}

	TArray<FSpawnEntry> SpawnEntries = ContainerData.SpawnEntries;
	int32 Count = FMath::RandRange(ContainerData.MinSpawnCount, ContainerData.MaxSpawnCount);

	UE_LOG(LogTemp, Warning, TEXT("Spawn Count : %d"), Count);

	for (int i = 0; i < Count; i++)
	{
		FName ItemID = GetRandomSpawnItem(SpawnEntries);

		if (SpawnItems.Contains(ItemID))
		{
			SpawnItems[ItemID] += 1;
		}
		else
		{
			SpawnItems.Add(ItemID, 1);
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

FName UContainerManager::GetRandomSpawnItem(const TArray<FSpawnEntry>& SpawnEntries)
{
	FName Default = FName(TEXT("Default"));

	if (!GameInstance)
	{
		return Default;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return Default;
	}

	UItemDataSubsystem* ItemDataSubsystem = GameInstance->GetSubsystem<UItemDataSubsystem>();
	if (!ItemDataSubsystem)
	{
		return Default;
	}

	FName KeyCardID = DataAsset->GetKeyCardID();
	float Prob = FMath::RandRange(0.0f, 1.0f);
	float Sum{};

	for (const FSpawnEntry& SpawnEntry : SpawnEntries)
	{
		Sum += SpawnEntry.Prob;

		if (Sum >= Prob)
		{
			FName ItemID = SpawnEntry.ID;

			if (const FItemDataRow* ItemData = ItemDataSubsystem->GetItemData(ItemID))
			{
				if (ItemID == KeyCardID)
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

				return ItemID;
			}
		}
	}

	return Default;
}

bool UContainerManager::GetContainerData(FName ContainerID, FSpawnData& Data) const
{
	if (ContainerDatas.Contains(ContainerID))
	{
		Data = ContainerDatas[ContainerID];

		return true;
	}

	return false;
}

void UContainerManager::CleanSetting()
{
	ContainerDatas.Empty();
	ContainerByRegion.Empty();
}
