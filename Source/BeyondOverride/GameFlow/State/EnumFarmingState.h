// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnumFarmingState.generated.h"

UENUM(BlueprintType)
enum class EFarmingState : uint8
{
	None,
	Begin,
	Progress,
	End
};
