#pragma once

#include "CoreMinimal.h"

#include "LoadingTipDataRow.generated.h"

USTRUCT(BlueprintType)
struct BEYONDOVERRIDE_API FLoadingTipDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI|Loading")
	FString LoadingTip;
};
