#pragma once

#include "CoreMinimal.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"

#include "ItemRarityStyleDataAsset.generated.h"

UCLASS(BlueprintType)
class BEYONDOVERRIDE_API UItemRarityStyleDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rarity")
	TMap<EItemRarity, FSlateBrush> RarityBrushMap;
};
