// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Kismet/BlueprintFunctionLibrary.h"

// Add include
#include "Monster/Structs/SystemParams.h"

// UHT Header
#include "BFLCircleSerchPoint.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBFLCircleSerchPoint : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "CircleSerchPoint")
	static FVector CircleSerch(bool CanLook, bool NotReturn, AActor* Target, FSerchValues Values, UObject* WorldContextObject);
};
