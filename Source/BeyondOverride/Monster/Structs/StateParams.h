// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "StateParams.generated.h"

USTRUCT()
struct FFlagInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EFlag Flag;

	UPROPERTY(VisibleAnywhere)
	bool Complete = false;

	UPROPERTY(VisibleAnywhere)
	float CallTime;

	UPROPERTY(VisibleAnywhere)
	bool FlagType = false;
};
