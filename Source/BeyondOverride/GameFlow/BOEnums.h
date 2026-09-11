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
	Farming
};

UENUM(BlueprintType)
enum class ELevel : uint8
{
	Bunker,
	Main
};

UENUM(BlueprintType)
enum class EFarmingState : uint8
{
	None,
	Begin,
	Progress,
	End
};

UENUM(BlueprintType)
enum class EFarmingResult : uint8
{
	None,
	Success,
	Fail
};
