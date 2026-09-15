#pragma once

#include "CoreMinimal.h"

#include "FireMode.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto, // 단발 사격
	FullAuto, // 연발 사격
	// Burst, // 점사
};
