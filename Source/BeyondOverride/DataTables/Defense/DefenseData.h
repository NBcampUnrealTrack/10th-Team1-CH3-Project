// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Farming/SpawnData.h"

#include "DefenseData.generated.h"

USTRUCT(BlueprintType)
struct FDefenseData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float DurationRatio = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	TArray<FSpawnEntry> SpawnEntries;
};
