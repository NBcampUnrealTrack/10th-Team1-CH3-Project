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
	TSubclassOf<ULoadingScreenWidget> GetLoadingScreenWidgetClass() const;
	void GetLoadingImages(TArray<TObjectPtr<UTexture2D>>& Images) const;
	float GetLoadingScreenUpdateTime() const;
	float GetLoadingImageChangeTime() const;
	float GetLoadingImageUpdateInterval() const;
	float GetLoadingProgressUpdateInterval() const;

	void GetLevels(TMap<ELevel, TSoftObjectPtr<UWorld>>& Data) const;
	void GetRegions(TArray<FName>& Data) const;
	void GetBasicEquipments(TArray<FName>& Data) const;
	FName GetKeyCardID() const;
	float GetExitActivateProb() const;
	float GetTotalDefenseTime() const;

	UDataTable* GetSpawnVolumeDataTable() const;
	UDataTable* GetPhaseDataTable() const;
	UDataTable* GetMonsterDataTable() const;
	UDataTable* GetContainerDataTable() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TArray<TObjectPtr<UTexture2D>> LoadingImages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	float LoadingScreenUpdateTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	float LoadingImageChangeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	float LoadingImageUpdateInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	float LoadingProgressUpdateInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TMap<ELevel, TSoftObjectPtr<UWorld>> Levels;

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
	UDataTable* SpawnVolumeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* PhaseDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* MonsterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* ContainerDataTable;
};
