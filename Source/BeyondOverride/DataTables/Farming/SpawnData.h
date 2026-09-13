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
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float Prob = 0.0f;
};

USTRUCT(BlueprintType)
struct FSpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName RegionId = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnCount = 0;
};
