// 26/09/28 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Kismet/BlueprintFunctionLibrary.h"

// UHT Header
#include "BFLMonsterStorageSpawn.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBFLMonsterStorageSpawn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

  public:
	UBFLMonsterStorageSpawn();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	static void StorageSpawn(FVector SpawnLocation,
							 FName StorageTarget,
							 UObject* WorldContextObject);
};
