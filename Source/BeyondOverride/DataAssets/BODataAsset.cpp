// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/BODataAsset.h"

#include "Logging/BOLog.h"

void UBODataAsset::GetLevels(TMap<ELevel, TObjectPtr<UWorld>>& Data) const
{
	UE_LOG(LogGameFlow, Warning, TEXT("Get Levels"));
	Data = Levels;
}

void UBODataAsset::GetRegions(TArray<FName>& Data) const
{
	Data = Regions;
}

void UBODataAsset::GetBasicEquipments(TArray<FName>& Data) const
{
	Data = BasicEquipments;
}

TSubclassOf<ULoadingScreenWidget> UBODataAsset::GetLoadingScreenWidgetClass() const
{
	return LoadingScreenWidgetClass;
}

void UBODataAsset::GetLoadingImages(TArray<TObjectPtr<UTexture2D>>& Images) const
{
	Images = LoadingImages;
}

float UBODataAsset::GetLoadingScreenUpdateTime() const
{
	return LoadingScreenUpdateTime;
}

float UBODataAsset::GetLoadingImageChangeTime() const
{
	return LoadingImageChangeTime;
}

float UBODataAsset::GetLoadingImageUpdateInterval() const
{
	return LoadingImageUpdateInterval;
}

float UBODataAsset::GetLoadingProgressUpdateInterval() const
{
	return LoadingProgressUpdateInterval;
}

FName UBODataAsset::GetKeyCardID() const
{
	return KeyCardID;
}

float UBODataAsset::GetExitActivateProb() const
{
	return ExitActivateProb;
}

float UBODataAsset::GetTotalDefenseTime() const
{
	return TotalDefenseTime;
}

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
