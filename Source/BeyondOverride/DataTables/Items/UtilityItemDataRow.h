#pragma once

#include "CoreMinimal.h"

#include "UtilityItemDataRow.generated.h"

enum class EUtilityType : uint8;

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FUtilityItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	EUtilityType EffectType; // 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float EffectAmount; // 효과량
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float UseDuration; // 사용에 걸리는 시간
};
