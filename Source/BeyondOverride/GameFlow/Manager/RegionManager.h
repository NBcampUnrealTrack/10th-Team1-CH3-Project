// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Farming/RegionData.h"
#include "DataTables/Farming/SpawnData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "RegionManager.generated.h"

class UBODataAsset;

UCLASS()
class BEYONDOVERRIDE_API URegionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadRegionData();

  public:
	void InitSetting();
	void CleanSetting();
	bool GetRegiondata(FName RegionId, FRegionData& Data) const;

  private:
	TMap<FName, FRegionData> RegionDatas;
};
