// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "StateEnums.generated.h"

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1 UMETA(DisplayName = "Phase1"),
	Phase2 UMETA(DisplayName = "Phase2"),
};

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Chase UMETA(DisplayName = "Chase"),
	Patrol UMETA(DisplayName = "Patrol"),
	Attack UMETA(DisplayName = "Attack"),
	Atmosphere UMETA(DisplayName = "Atmosphere"),
	StandOffMove UMETA(DisplayName = "StandOffMove"),
	StandOffWait UMETA(DisplayName = "StandOffWait"),
	LocationPatrol UMETA(DisplayName = "LocationPatrol"),
};

UENUM(BlueprintType)
enum class EFlag : uint8
{
	TakeDamage UMETA(DisplayName = "TakeDamage"),
	Calling UMETA(DisplayName = "Calling"),
	Hearing UMETA(DisplayName = "Hearing"),
};
