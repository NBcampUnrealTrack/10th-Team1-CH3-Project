// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Farming/SpawnData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ContainerManager.generated.h"

class UBOGameInstance;
class UItemInstanceBase;
class AStorageContainerActor;

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

	void ActivateContainer();
	void GetSpawnItems(AStorageContainerActor* Container, TArray<TObjectPtr<UItemInstanceBase>>& Items);
	FName GetRandomSpawnItem(const TArray<FSpawnEntry>& SpawnEntries);

	bool GetContainerData(FName ContainerID, FSpawnData& Data) const;

	void CleanSetting();

  private:
	TObjectPtr<UBOGameInstance> GameInstance;

	bool bShouldSpawnKeyCard;

	TMap<FName, FSpawnData> ContainerDatas;
	TMap<FName, TArray<TObjectPtr<AStorageContainerActor>>> ContainerByRegion;
};
