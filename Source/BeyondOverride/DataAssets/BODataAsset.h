// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "BODataAsset.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UBODataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UDataTable* GetRegionDataTable() const;
	UDataTable* GetSpawnVolumeDataTable() const;
	UDataTable* GetPhaseDataTable() const;
	UDataTable* GetMonsterDataTable() const;
	UDataTable* GetContainerDataTable() const;

  public:
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
