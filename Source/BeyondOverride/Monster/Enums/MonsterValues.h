// 26/09/27 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "MonsterValues.generated.h"

UENUM(BlueprintType)
enum class EBossPattern : uint8
{
	Missile UMETA(DisplayName = "Missile"),
	Range UMETA(DisplayName = "Range"),
	Melee UMETA(DisplayName = "Melee"),
};
