// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/NPCManager.h"

#include "DataAssets/BODataAsset.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Logging/BOLog.h"

UNPCManager::UNPCManager()
{
	NPCDatas.Empty();
	NPCShopItems.Empty();

	LoadNPCData();
	LoadNPCShopData();
}

void UNPCManager::LoadNPCData()
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

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	UDataTable* NPCDataTable = DataAsset->GetNPCDataTable();
	if (!NPCDataTable)
	{
		return;
	}

	TMap<FName, uint8*> AllRows = NPCDataTable->GetRowMap();

	for (TPair<FName, uint8*> Pair : AllRows)
	{
		if (FNPCData* Row = reinterpret_cast<FNPCData*>(Pair.Value))
		{
			FName NPCID = Pair.Key;

			NPCDatas.Add(NPCID, *Row);
		}
	}
}

void UNPCManager::LoadNPCShopData()
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

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	UDataTable* NPCShopDataTable = DataAsset->GetNPCShopDataTable();
	if (!NPCShopDataTable)
	{
		return;
	}

	TMap<FName, uint8*> AllRows = NPCShopDataTable->GetRowMap();

	for (TPair<FName, uint8*> Pair : AllRows)
	{
		if (FNPCShopData* Row = reinterpret_cast<FNPCShopData*>(Pair.Value))
		{
			FName NPCID = Pair.Key;

			NPCShopDatas.Add(NPCID, *Row);
			SetNPCShopItems(NPCID, (*Row).ShopItems);
		}
	}
}

void UNPCManager::SetNPCShopItems(FName NPCID, const TArray<FName>& ShopItems)
{
	TArray<UItemInstanceBase*> Items{};
	FItemFactory ItemFactory{};

	for (FName ItemID : ShopItems)
	{
		if (UItemInstanceBase* ItemInstance = ItemFactory.CreateItemInstance(this, ItemID, 10))
		{
			Items.Add(ItemInstance);
		}
	}

	NPCShopItems.Add(NPCID, Items);
}

void UNPCManager::GetNPCData(FName NPCID, FNPCData& Data) const
{
	if (NPCDatas.Contains(NPCID))
	{
		Data = NPCDatas[NPCID];
	}
}

void UNPCManager::GetNPCShopData(FName NPCID, FNPCShopData& Data) const
{
	if (NPCShopDatas.Contains(NPCID))
	{
		Data = NPCShopDatas[NPCID];
	}
}

void UNPCManager::GetNPCShopItems(FName NPCID, TArray<UItemInstanceBase*>& Items)
{
	Items.Empty();

	if (NPCShopItems.Contains(NPCID))
	{
		Items = NPCShopItems[NPCID];
	}
}
