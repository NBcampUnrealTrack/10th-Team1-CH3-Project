// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BOEnums.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Begin,
	Playing,
	End
};

UENUM(BlueprintType)
enum class EPlayingState : uint8
{
	None,
	Bunker,
	Farming,
	Defense
};

UENUM(BlueprintType)
enum class EDeathLocation : uint8
{
	None,
	Bunker,
	Main,
	AIBuilding
};

UENUM(BlueprintType)
enum class ELevel : uint8
{
	None,
	Basic,
	Bunker,
	Main,
	AIBuilding
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	None,
	Begin,
	Progress,
	End
};

UENUM(BlueprintType)
enum class EStageResult : uint8
{
	None,
	Success,
	Fail,
	Clear
};

UENUM(BlueprintType)
enum class ESaveType : uint8
{
	Partial,
	All
};
