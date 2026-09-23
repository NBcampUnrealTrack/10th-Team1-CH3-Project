// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataAssets/BODataAsset.h"
#include "DataTables/Monster/MonsterInfo.h"
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
	void End();
	void Exit();
	void StartFarming();
	void EndFarming(EFarmingResult Result);
	void Die();
	void EnterServerRoom();

	void OpenLevel(ELevel Level);
	void ShowLoadingScreenWidget(bool IsNew);

	UFUNCTION(BlueprintCallable)
	void OnPostLoadMap(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void HideLoadingScreenWidget();

	void OnCharacterPrepared();
	void OnLevelPrepared();

	void SavePlayerData();
	void SaveStorageData();
	void SaveFarmingData();
	void SaveSurvivalTimeData();
	void SaveCombatData();

  public:
	UBODataAsset* GetBODataAsset() const;

	void GetMonsterData(FName Id, FMonsterInfo& Data) const; // move to monster spawn system

	EGameState GetGameState() const;
	EPlayingState GetPlayingState() const;
	EDeathLocation GetDeathLocation() const;
	EFarmingResult GetFarmingResult() const;

	float GetTotalSurvivalTime() const;
	float GetSurvivalTime() const;
	int32 GetFarmingCount() const;
	int32 GetDeathCount() const;
	void GetTotalKilledMonsters(TMap<FName, int32>& Data) const;
	void GetKilledMonsters(TMap<FName, int32>& Data) const;
	FName GetKillerMonster() const;

	float GetCurHealth() const;
	float GetMaxHealth() const;
	float GetCurShield() const;
	float GetMaxShield() const;

	bool GetIsLevelPreparing() const;

	TArray<UItemInstanceBase*> GetPlayerItemInventory() const;
	TArray<UItemInstanceBase*> GetPlayerEquipmentInventory() const;
	TArray<UItemInstanceBase*> GetStorageInventory() const;
	TMap<FName, FMonsterInfo> GetMonsterDatas() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	UBODataAsset* BODataAsset;

  private:
	EGameState GameState;
	EPlayingState PlayingState;
	EDeathLocation DeathLocation;
	EFarmingResult FarmingResult;
	ELevel CurLevel;

	float TotalSurvivalTime;
	float SurvivalTime;
	int32 FarmingCount;
	int32 DeathCount;
	TMap<FName, int32> TotalKilledMonsters;
	TMap<FName, int32> KilledMonsters;
	FName KillerMonster;

	int32 CurHealth;
	int32 MaxHealth;
	int32 CurShield;
	int32 MaxShield;

	bool IsLevelPreparing;
	TMap<ELevel, TSoftObjectPtr<UWorld>> Levels;

	FTimerHandle HideLoadingScreenTimer;

	UPROPERTY()
	TArray<UItemInstanceBase*> PlayerItemInventory;
	UPROPERTY()
	TArray<UItemInstanceBase*> PlayerEquipmentInventory;
	UPROPERTY()
	TArray<UItemInstanceBase*> StorageInventory;
	UPROPERTY()
	TMap<FName, FMonsterInfo> MonsterDatas; // move to monster spawn system
};
