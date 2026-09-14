// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataAssets/BODataAsset.h"
#include "Engine/GameInstance.h"
#include "Enums/BOEnums.h"

#include "BOGameInstance.generated.h"

class UItemInstanceBase;

UCLASS()
class BEYONDOVERRIDE_API UBOGameInstance : public UGameInstance
{
	GENERATED_BODY()

  private:
	virtual void Init() override;
	void LoadMonsterData();

  public:
	void InitSetting();
	void Start();
	void Restart();
	void Exit();
	void StartFarming();
	void EndFarming(EFarmingResult Result);
	void OpenLevel(ELevel Level);

	void SavePlayerData();
	void SaveStorageData();
	void SaveFarmingData();
	void CheckKeyCard();

  public:
	UBODataAsset* GetBODataAsset() const;
	void GetLevels(TMap<ELevel, FName>& Data) const;
	void GetRegions(TArray<FName>& Data) const;
	void GetBasicEquipments(TArray<FName>& Data) const;
	float GetExitActivateProb() const;
	// void GetAIData(FName Id, FAIData& Data) const;

	EGameState GetGameState() const;
	EPlayingState GetPlayingState() const;
	EFarmingResult GetFarmingResult() const;

	float GetTotalSurvivalTime() const;
	float GetSurvivalTime() const;
	void GetTotalKilledMonsters(TMap<FName, int32>& Data) const;
	void GetKilledMonsters(TMap<FName, int32>& Data) const;
	FName GetKillerMonster() const;

	float GetCurrentHealth() const;
	float GetCurrentShield() const;
	int32 GetTotalMoney() const;

	bool GetIsKeyCardAcquired() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	UBODataAsset* BODataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TMap<ELevel, FName> Levels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TArray<FName> Regions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TArray<FName> BasicEquipments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float ExitActivateProb;

  private:
	EGameState GameState;
	EPlayingState PlayingState;
	EFarmingResult FarmingResult;

	float TotalSurvivalTime;
	float SurvivalTime;
	TMap<FName, int32> TotalKilledMonsters;
	TMap<FName, int32> KilledMonsters;
	FName KillerMonster;

	int32 CurHealth;
	int32 CurShield;
	int32 TotalMoney;

	TArray<TObjectPtr<UItemInstanceBase>> PlayerItemInventory;
	TArray<TObjectPtr<UItemInstanceBase>> PlayerEquipmentInventory;
	TArray<TObjectPtr<UItemInstanceBase>> StorageInventory;

	bool IsKeyCardAcquired;

	// TMap<FName, FMonsterData> MonsterDatas;
};
