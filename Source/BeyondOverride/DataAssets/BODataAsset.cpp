// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/BODataAsset.h"

UDataTable* UBODataAsset::GetRegionDataTable() const
{
	return RegionDataTable;
}

UDataTable* UBODataAsset::GetSpawnVolumeDataTable() const
{
	return SpawnVolumeDataTable;
}

UDataTable* UBODataAsset::GetPhaseDataTable() const
{
	return PhaseDataTable;
}

UDataTable* UBODataAsset::GetMonsterDataTable() const
{
	return MonsterDataTable;
}

UDataTable* UBODataAsset::GetContainerDataTable() const
{
	return ContainerDataTable;
}
