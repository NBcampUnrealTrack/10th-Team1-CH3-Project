// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/BODataAsset.h"

#include "Logging/BOLog.h"

void UBODataAsset::GetLevels(TMap<ELevel, TSoftObjectPtr<UWorld>>& Data) const
{
	UE_LOG(LogGameFlow, Warning, TEXT("Get Levels"));
	Data = Levels;
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

int32 UBODataAsset::GetMonsterSpawnInterval() const
{
	return MonsterSpawnInterval;
}

void UBODataAsset::GetSupplies(TArray<FName>& Data) const
{
	Data = Supplies;
}

int32 UBODataAsset::GetSupplySpawnCount() const
{
	return SupplySpawnCount;
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

UDataTable* UBODataAsset::GetDefenseDataTable() const
{
	return DefenseDataTable;
}

UDataTable* UBODataAsset::GetLoadingTipTable() const
{
	return LoadingTipTable;
}
