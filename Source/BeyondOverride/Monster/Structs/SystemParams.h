// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "SystemParams.generated.h"

USTRUCT(BlueprintType)
struct FSerchValues
{

	GENERATED_BODY()

  public:
	size_t Smaple;

	float Radius;
	float XYRange;
	float ZRange;

	FVector Centor = FVector::ZeroVector;

	float BaseAngle = 0;
};
