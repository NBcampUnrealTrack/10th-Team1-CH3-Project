// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpawnStruct.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float Probability = 0.0f;
};

USTRUCT(BlueprintType)
struct FSpawnVolumeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVolume")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVolume")
	TArray<FSpawnData> SpawnableAIs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnVolume")
	int32 SpawnAICount = 0;
};

USTRUCT(BlueprintType)
struct FContainerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	TArray<FSpawnData> SpawnableItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container")
	int32 SpawnItemCount = 0;
};
