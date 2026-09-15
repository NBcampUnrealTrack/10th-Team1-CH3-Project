#pragma once

#include "CoreMinimal.h"

#include "EquipmentSlot.generated.h"

// 플레이어의 장비 슬롯 타입
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Unarmed,   // Unarmed (only hand)
	Primary,   // Primary Ranged Weapon
	Secondary, // Secondary Ranged Weapon
	Melee,     // Melee Weapon
	Throwable, // Throwable Item
	Effect,    // Effect Item
	Bag,
	Shield
};
