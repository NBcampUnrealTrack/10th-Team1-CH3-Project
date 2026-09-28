// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/NPC/NPCData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "NPCManager.generated.h"

class UItemInstanceBase;

UCLASS()
class BEYONDOVERRIDE_API UNPCManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	UNPCManager();

  private:
	void LoadNPCData();
	void LoadNPCShopData();
	void SetNPCShopItems(FName NPCID);

  public:
	void GetNPCData(FName NPCID, FNPCData& Data) const;
	void GetNPCShopData(FName NPCID, FNPCShopData& Data) const;
	void GetNPCShopItems(FName NPCID, TArray<UItemInstanceBase*>& Items);

  private:
	TMap<FName, FNPCData> NPCDatas;
	TMap<FName, FNPCShopData> NPCShopDatas;

	UPROPERTY()
	TArray<UItemInstanceBase*> NPCShopItems;
};
