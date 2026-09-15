// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RegionData.generated.h"

USTRUCT(BlueprintType)
struct FRegionData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName Id = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName SpawnVolumeId = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FName ExitId = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float ContainerActivateProb = 0.0f;
};
