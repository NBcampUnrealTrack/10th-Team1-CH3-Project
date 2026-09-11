// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Data/GameDataAsset.h"

UDataTable* UGameDataAsset::GetSpawnVolumeDataTable() const
{
	return SpawnVolumeDataTable;
}

UDataTable* UGameDataAsset::GetAIDataTable() const
{
	return AIDataTable;
}

UDataTable* UGameDataAsset::GetContainerDataTable() const
{
	return ContainerDataTable;
}
