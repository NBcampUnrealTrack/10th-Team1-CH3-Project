#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/ItemInstanceBase.h"

#include "EquippableItemInstance.generated.h"

class UEquippableItemDataAsset;

UCLASS()
class UEquippableItemInstance : public UItemInstanceBase
{
	GENERATED_BODY()

  protected:
	TObjectPtr<const UEquippableItemDataAsset> EquippableItemData;

  public:
	UEquippableItemInstance();

	// Getters
	const UEquippableItemDataAsset* GetEquippableItemData() const;
};
