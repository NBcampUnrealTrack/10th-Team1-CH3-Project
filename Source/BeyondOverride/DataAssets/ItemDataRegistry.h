#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "ItemDataRegistry.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UItemDataRegistry : public UDataAsset
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> ItemTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> EquippableItemTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDataTable> RangeWeaponTable;
};
