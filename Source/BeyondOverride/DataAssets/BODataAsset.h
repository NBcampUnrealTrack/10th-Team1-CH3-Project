// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Enums/BOEnums.h"
#include "UI/Widgets/LoadingScreenWidget.h"

#include "BODataAsset.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UBODataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	void GetLevels(TMap<ELevel, FName>& Data) const;
	void GetRegions(TArray<FName>& Data) const;
	void GetBasicEquipments(TArray<FName>& Data) const;
	TSubclassOf<ULoadingScreenWidget> GetLoadingScreenWidgetClass() const;
	FName GetKeyCardID() const;
	float GetExitActivateProb() const;
	float GetTotalDefenseTime() const;

	UDataTable* GetRegionDataTable() const;
	UDataTable* GetSpawnVolumeDataTable() const;
	UDataTable* GetPhaseDataTable() const;
	UDataTable* GetMonsterDataTable() const;
	UDataTable* GetContainerDataTable() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TMap<ELevel, FName> Levels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TArray<FName> Regions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TArray<FName> BasicEquipments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FName KeyCardID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float ExitActivateProb;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	float TotalDefenseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* RegionDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* SpawnVolumeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* PhaseDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* MonsterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* ContainerDataTable;
};
