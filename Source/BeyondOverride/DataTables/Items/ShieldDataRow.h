#pragma once

#include "CoreMinimal.h"

#include "ShieldDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FShieldDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 MaxShield = 0; // 최대 실드량
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float ShieldRegenDelay = 0.f; // 실드 회복 시작 딜레이
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float ShieldRegenInterval = 0.f; // 실드 회복 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 ShieldRegenAmount = 0; // 회복 간격 당 실드 회복량
};
