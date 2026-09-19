// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "SystemEnums.generated.h"

UENUM(BlueprintType)
enum class ECallType : uint8
{
	LocationPatrol UMETA(DisplayName = "LocationPatrol"),
	Attack UMETA(DisplayName = "Attack"),
};
