// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnData.h"

#include "PhaseData.generated.h"

USTRUCT(BlueprintType)
struct FPhaseEntry
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	int32 PhaseIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	TArray<FSpawnEntry> SpawnEntries;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	int32 SpawnCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	float Duration = 0.0f;
};

USTRUCT(BlueprintType)
struct FPhaseData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	FName SpawnVolumeId = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
	TArray<FPhaseEntry> PhaseEntries;
};
