// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/NPCManager.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"

UNPCManager::UNPCManager()
{
	NPCDatas.Empty();

	LoadNPCData();
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

void UNPCManager::GetNPCData(FName NPCID, FNPCData& Data) const
{
	if (NPCDatas.Contains(NPCID))
	{
		Data = NPCDatas[NPCID];
	}
}
