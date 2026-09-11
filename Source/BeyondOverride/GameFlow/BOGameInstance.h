// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BOEnums.h"
#include "CoreMinimal.h"

#include "Data/GameDataAsset.h"
#include "Data/SpawnStruct.h"
#include "Engine/GameInstance.h"

#include "BOGameInstance.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBOGameInstance : public UGameInstance
{
	GENERATED_BODY()

  public:
	virtual void Init() override;

  private:
	void LoadSpawnVolumeData();
	void LoadAIData();
	void LoadContainerData();

  public:
	void Start();
	void Restart();
	void Exit();
	void StartFarming();
	void EndFarming(EFarmingResult Result);
	void OpenLevel(ELevel Level);
	void SavePlayerData();

	// TMap<ELevel, FName> GetLevels() const;
	// TArray<FName> GetRegions() const;
	void GetSpawnVolumeData(FName Id, FSpawnStruct& Data);
	// void GetAIData(FName Id, FAIData& Data);
	void GetContainerData(FName Id, FSpawnStruct& Data);
	EGameState GetGameState() const;
	EPlayingState GetPlayingState() const;
	EFarmingResult GetFarmingResult() const;
	float GetCurrentHealth() const;
	float GetCurrentShield() const;
	int32 GetTotalMoney() const;

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<ELevel, FName> Levels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FName> Regions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UGameDataAsset* GameDataAsset;

  private:
	EGameState GameState;
	EPlayingState PlayingState;
	EFarmingResult FarmingResult;
	float TotalSurvivalTime;
	int32 CurrentHealth;
	int32 CurrentShield;
	int32 TotalMoney;
	// TArray<FInventorySlot> Inventory;

	TMap<FName, FSpawnStruct> SpawnVolumeDatas;
	// TMap<FName, FAIData> AIDatas;
	TMap<FName, FSpawnStruct> ContainerDatas;
};
