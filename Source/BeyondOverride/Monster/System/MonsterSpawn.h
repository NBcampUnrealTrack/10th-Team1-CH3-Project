
// 26/09/16 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "UObject/NoExportTypes.h"

// UHT Header
#include "MonsterSpawn.generated.h"

class AMonsterCharacter;

UCLASS()
class BEYONDOVERRIDE_API UMonsterSpawn : public UObject
{
	GENERATED_BODY()

  public:
	UMonsterSpawn();

	UFUNCTION(BlueprintCallable)
	void MonsterSpawn(TSubclassOf<AMonsterCharacter> MonsterClass, FVector Location, FName ID);

	TSubclassOf<AMonsterCharacter> MonsterClass;
};
