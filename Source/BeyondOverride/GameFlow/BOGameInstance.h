// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"

#include "BOGameInstance.generated.h"

UENUM(BlueprintType)
enum class ELevel : uint8
{
	Bunker,
	Main
};

UCLASS()
class BEYONDOVERRIDE_API UBOGameInstance : public UGameInstance
{
	GENERATED_BODY()

  private:
	void LoadSpawnVolumeData();

  public:
	void RestartBO();
	void ExitBO();
	void OpenLevel(ELevel Level);
	void SavePlayerData();

	// void GetSpawnVolumeData(FName Id, FSpawnVolumeData& Data);
	float GetCurrentHealth() const;
	float GetCurrentShield() const;
	int32 GetTotalMoney() const;

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TArray<FName> Levels;

  private:
	float TotalSurvivalTime;
	int32 CurrentHealth;
	int32 CurrentShield;
	int32 TotalMoney;
	// TArray<FInventorySlot> Inventory;
};
