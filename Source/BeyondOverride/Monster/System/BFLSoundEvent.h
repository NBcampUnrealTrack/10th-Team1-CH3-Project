// 26/09/09 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Kismet/BlueprintFunctionLibrary.h"

// UHT Header
#include "BFLSoundEvent.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBFLSoundEvent : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
  public:
	static void NoisePlay(FVector Location, float Radius, UObject* WorldContextObject);
};
