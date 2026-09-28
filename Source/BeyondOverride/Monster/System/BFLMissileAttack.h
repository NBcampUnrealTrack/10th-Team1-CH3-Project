// 26/09/19 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Kismet/BlueprintFunctionLibrary.h"

// UHT Header
#include "BFLMissileAttack.generated.h"

class AAttackMissileActor;

UCLASS()
class BEYONDOVERRIDE_API UBFLMissileAttack : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

  public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	static void MissileAttack(FVector SpawnLocation,
							  FVector AttackPoint,
							  float FireAngle,
							  float FlightTime,
							  ACharacter* ThisOwner,
							  UObject* WorldContextObject);
};
