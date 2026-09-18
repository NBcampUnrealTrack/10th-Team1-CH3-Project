// 26/09/15 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "MonsterSense.generated.h"

USTRUCT(BlueprintType)
struct FMonsterSense : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HearSenseSize = 1750.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightSenseSize = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseSightSize = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisionAngleDegrees = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Memorize = 5.0f;
};
