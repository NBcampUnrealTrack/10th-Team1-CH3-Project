// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "GameDataAsset.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UGameDataAsset : public UDataAsset
{
	GENERATED_BODY()

  public:
	UDataTable* GetSpawnVolumeDataTable();
	UDataTable* GetContainerDataTable();

  public:
	UDataTable* SpawnVolumeDataTable;
	UDataTable* ContainerDataTable;
};
