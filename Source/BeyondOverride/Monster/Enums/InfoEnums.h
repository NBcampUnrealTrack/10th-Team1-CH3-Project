// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "InfoEnums.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	Special UMETA(DisplayName = "Special"),
	Range UMETA(DisplayName = "Range"),
	Melee UMETA(DisplayName = "Melee"),
	Fly UMETA(DisplayName = "Fly"),
	Boss UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class EAirNavState : uint8
{
	Fail UMETA(DisplayName = "Fail"),
	Continue UMETA(DisplayName = "Continue"),
	Complete UMETA(DisplayName = "Complete")
};
