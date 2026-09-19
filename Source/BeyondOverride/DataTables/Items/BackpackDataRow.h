#pragma once

#include "CoreMinimal.h"

#include "BackpackDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FBackpackDataRow : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	int32 SlotCount = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats")
	float WeightCapacity = 0.f;
};
