// 26/09/15 Copyright Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "Engine/DataAsset.h"
#include "Monster/DataTable/MonsterInfo.h"

// UHT Header
#include "MonsterDataAsset.generated.h"

UCLASS(BlueprintType)
class BEYONDOVERRIDE_API UMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	UDataTable* MeshTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	UDataTable* StatTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	UDataTable* SenseTable;
};
