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

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	Random UMETA(DisplayName = "Random"),
	Point UMETA(DisplayName = "Point"),
	Stop UMETA(DisplayName = "Stop"),
};

UENUM(BlueprintType)
enum class EPointPatrolState : uint8
{
	Go UMETA(DisplayName = "Go"),
	Return UMETA(DisplayName = "Return"),
};
