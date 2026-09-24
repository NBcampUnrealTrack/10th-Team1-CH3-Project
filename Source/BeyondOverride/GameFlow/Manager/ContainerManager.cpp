// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ContainerManager.h"

#include "RegionManager.h"

#include "Algo/RandomShuffle.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Spawn/SpawnVolume.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
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

	const TMap<FName, uint8*>& AllRows = ContainerDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& pair : AllRows)
	{
		FName RegionID = pair.Key;
		FContainerData* ContainerData = reinterpret_cast<FContainerData*>(pair.Value);

		ContainerDatas.Add(RegionID, *ContainerData);
	}
}

void UContainerManager::InitSetting()
{
	bShouldSpawnKeyCard = !IsKeyCardAcquired();
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
		if (Item && Item->GetItemID() == KeyCardID)
		{
			return true;
		}
	}

	return false;
}

void UContainerManager::ActivateContainers(TObjectPtr<ASpawnVolume> OverlappedSpawnVolume)
{
	if (!IsValid(OverlappedSpawnVolume))
	{
		return;
	}

	FName RegionID = OverlappedSpawnVolume->GetRegionID();

	if (!ContainerDatas.Contains(RegionID))
	{
		return;
	}

	UE_LOG(LogGameFlow, Warning, TEXT("Container Manager : Activate Containers"));

	FContainerData ContainerData = ContainerDatas[RegionID];
	float Prob = ContainerData.ContainerActivateProb;

	TArray<TObjectPtr<AStorageContainerActor>> Containers{};

	TArray<AActor*> AllActors{};
	OverlappedSpawnVolume->GetOverlappingActors(AllActors, AStorageContainerActor::StaticClass());

	UE_LOG(LogGameFlow, Warning, TEXT("Container Count : %d"), AllActors.Num());

	for (AActor* Actor : AllActors)
	{
		if (AStorageContainerActor* Container = Cast<AStorageContainerActor>(Actor))
		{
			Containers.Add(Container);
		}
	}

	int32 Size = Containers.Num();
	int32 Count = FMath::RoundToInt(Size * Prob);

	Algo::RandomShuffle(Containers);

	for (int i = 0; i < Count; i++)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Set Container Items"));

		TObjectPtr<AStorageContainerActor> Container = Containers[i];

		TArray<TObjectPtr<UItemInstanceBase>> Items{};
		GetSpawnItems(ContainerData, Items);

		Container->SetItems(Items);
	}

	for (int i = Count; i < Size; i++)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Set Container Single Item"));

		TObjectPtr<AStorageContainerActor> Container = Containers[i];

		TArray<TObjectPtr<UItemInstanceBase>> Items{};
		TObjectPtr<UItemInstanceBase> Item = GetSpawnItem(ContainerData);
		Items.Add(Item);

		Container->SetItems(Items);
	}
}

void UContainerManager::GetSpawnItems(const FContainerData ContainerData, TArray<TObjectPtr<UItemInstanceBase>>& Items)
{
	Items.Empty();
	TMap<FName, int32> SpawnItems{};

	TArray<FSpawnEntry> SpawnEntries = ContainerData.SpawnEntries;
	int32 Count = FMath::RandRange(ContainerData.MinSpawnCount, ContainerData.MaxSpawnCount);

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
		for (int i = 0; i < Item.Value; i++)
		{
			if (UItemInstanceBase* ItemInstanceBase = ItemFactory.CreateItemInstance(this, Item.Key))
			{
				UE_LOG(LogGameFlow, Warning, TEXT("Add Item"));
				Items.Add(ItemInstanceBase);
			}
		}
	}
}

TObjectPtr<UItemInstanceBase> UContainerManager::GetSpawnItem(const FContainerData& ContainerData)
{
	TArray<FSpawnEntry> SpawnEntries = ContainerData.SpawnEntries;
	FName ItemID = GetRandomSpawnItem(SpawnEntries);

	FItemFactory ItemFactory{};
	UItemInstanceBase* ItemInstanceBase = ItemFactory.CreateItemInstance(this, ItemID);

	return ItemInstanceBase;
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

void UContainerManager::CleanSetting()
{
	bShouldSpawnKeyCard = false;
}
