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
struct FSpawnStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName RegionId = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FSpawnData> SpawnableDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnCount = 0;
};
