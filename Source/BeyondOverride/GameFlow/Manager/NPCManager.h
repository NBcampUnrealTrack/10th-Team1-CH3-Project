// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/NPC/NPCData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "NPCManager.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UNPCManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	UNPCManager();

  private:
	void LoadNPCData();

  public:
	void GetNPCData(FName NPCID, FNPCData& Data) const;

  private:
	UPROPERTY()
	TMap<FName, FNPCData> NPCDatas;
};
