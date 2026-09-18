#pragma once

#include "CoreMinimal.h"

#include "Enums/UtilityType.h"

#include "UtilityItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FUtilityItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	EUtilityType EffectType = EUtilityType::None; // 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float EffectAmount = 0.f; // 효과량
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float UseDuration = 0.f; // 사용에 걸리는 시간
};
