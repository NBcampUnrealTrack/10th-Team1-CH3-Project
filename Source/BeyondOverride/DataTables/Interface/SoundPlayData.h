// 26/09/19 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// UHT Header
#include "SoundPlayData.generated.h"

USTRUCT(BlueprintType)
struct FSoundPlayData : public FTableRowBase
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> SoundTarget;
};
