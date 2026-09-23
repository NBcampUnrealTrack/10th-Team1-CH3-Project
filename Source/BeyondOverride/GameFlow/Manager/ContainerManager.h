// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Farming/SpawnData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ContainerManager.generated.h"

class UBOGameInstance;
class UItemInstanceBase;
class AStorageContainerActor;
class ASpawnVolume;

UCLASS()
class BEYONDOVERRIDE_API UContainerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadContainerData();

  public:
	void InitSetting();
	bool IsKeyCardAcquired() const;
	bool HasPlayerKeyCard() const;
	bool HasStorageKeyCard() const;

	void ActivateContainers(TObjectPtr<ASpawnVolume> OverlappedSpawnVolume);

	void GetSpawnItems(const FContainerData ContainerData, TArray<TObjectPtr<UItemInstanceBase>>& Items);
	TObjectPtr<UItemInstanceBase> GetSpawnItem(const FContainerData& ContainerData);
	FName GetRandomSpawnItem(const TArray<FSpawnEntry>& SpawnEntries);

	void CleanSetting();

  private:
	TObjectPtr<UBOGameInstance> GameInstance;

	bool bShouldSpawnKeyCard;

	TMap<FName, FContainerData> ContainerDatas;
};
