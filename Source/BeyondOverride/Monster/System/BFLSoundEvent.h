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
	UFUNCTION(BlueprintCallable, Category = "Noise Player")
	static void NoisePlay(FVector Location, float Radius, UObject* WorldContextObject);

	static void SoundPlay(FVector Location, FName SoundTarget, float Radius, UObject* WorldContextObject);

	static void SoundPlay(FVector Location, FName SoundTarget, float Radius, float Pitch, float Volume, UObject* WorldContextObject);

	static void SoundPlay(FVector Location, FName SoundTarget, float Radius, bool IsPlayer, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Sound Player", meta = (WorldContext = "WorldContextObject"))
	static void SoundPlay(FVector Location, FName SoundTarget, float Radius, float Pitch, float Volume, bool IsPlayer, UObject* WorldContextObject);
};
