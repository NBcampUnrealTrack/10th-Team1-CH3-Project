// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BOEnums.h"
#include "CoreMinimal.h"

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

  public:
	void Start();
	void Restart();
	void Exit();
	void StartFarming();
	void EndFarming(EFarmingResult Result);
	void OpenLevel(ELevel Level);
	void SavePlayerData();

	void GetSpawnVolumeData(FName Id, FSpawnVolumeData& Data);
	EGameState GetGameState() const;
	EPlayingState GetPlayingState() const;
	EFarmingResult GetFarmingResult() const;
	float GetCurrentHealth() const;
	float GetCurrentShield() const;
	int32 GetTotalMoney() const;

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TMap<ELevel, FName> Levels;

  private:
	EGameState GameState;
	EPlayingState PlayingState;
	EFarmingResult FarmingResult;
	float TotalSurvivalTime;
	int32 CurrentHealth;
	int32 CurrentShield;
	int32 TotalMoney;
	// TArray<FInventorySlot> Inventory;
};
