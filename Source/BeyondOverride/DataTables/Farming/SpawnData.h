// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SpawnData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName ID = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float Prob = 0.0f;
};

USTRUCT(BlueprintType)
struct FSpawnVolumeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MinSpawnCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxSpawnCount = 0;
};

USTRUCT(BlueprintType)
struct FContainerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float ContainerActivateProb = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MinSpawnCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxSpawnCount = 0;
};
