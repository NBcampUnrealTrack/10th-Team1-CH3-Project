// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Enums/BOEnums.h"
#include "UI/Widgets/LoadingScreenWidget.h"

#include "BODataAsset.generated.h"

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

	void GetBasicEquipments(TArray<FName>& Data) const;
	FName GetKeyCardID() const;
	float GetExitActivateProb() const;

	float GetTotalDefenseTime() const;
	int32 GetMonsterSpawnInterval() const;
	void GetSupplies(TArray<FName>& Data) const;
	int32 GetSupplySpawnCount() const;

	UDataTable* GetSpawnVolumeDataTable() const;
	UDataTable* GetPhaseDataTable() const;
	UDataTable* GetMonsterDataTable() const;
	UDataTable* GetContainerDataTable() const;
	UDataTable* GetDefenseDataTable() const;
	UDataTable* GetLoadingTipTable() const;

	UDataTable* GetNPCDataTable() const;
	UDataTable* GetNPCShopDataTable() const;

	UDataTable* GetTopicDataTable() const;
	UDataTable* GetDialogueInteractionDataTable() const;
	UDataTable* GetDialogueDataTable() const;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TArray<TObjectPtr<UTexture2D>> LoadingImages;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TObjectPtr<UDataTable> LoadingTipTable;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Farming")
	TArray<FName> BasicEquipments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Farming")
	FName KeyCardID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Farming")
	float ExitActivateProb;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Defense")
	float TotalDefenseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Defense")
	float MonsterSpawnInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Defense")
	TArray<FName> Supplies;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Defense")
	int32 SupplySpawnCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* SpawnVolumeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* PhaseDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* MonsterDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* ContainerDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DefenseDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* NPCDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* NPCShopDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* TopicDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DialogueInteractionDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	UDataTable* DialogueDataTable;
};
