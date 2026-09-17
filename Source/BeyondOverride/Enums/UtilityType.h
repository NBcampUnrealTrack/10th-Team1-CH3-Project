#pragma once

#include "CoreMinimal.h"

#include "UtilityType.generated.h"

UENUM(BlueprintType)
enum class EUtilityType : uint8
{
	None,
	HealHP,
	// HealShield,
	// HealStamina,
};
