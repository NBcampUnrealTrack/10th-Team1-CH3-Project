// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NPCEnums.generated.h"

UENUM(BlueprintType)
enum class ENPCInteractionOption : uint8
{
	None,
	Dialogue,
	Shop,
	Quest,
	Heal
};
