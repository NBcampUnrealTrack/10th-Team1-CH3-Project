#include "Items/Objects/ItemInstanceBase.h"

#include "Items/Actors/ItemPickupBase.h"
#include "Items/DataAssets/ItemDataAsset.h"

UItemInstanceBase::UItemInstanceBase()
{
	ItemData = nullptr;

	StackCount = 1;
}

AItemPickupBase* UItemInstanceBase::SpawnPickup(
	const FVector& Location,
	const FRotator& Rotation)
{
	// TODO

	return nullptr;
}

const UItemDataAsset* UItemInstanceBase::GetItemData() const
{
	return ItemData;
}

int32 UItemInstanceBase::GetStackCount() const
{
	return StackCount;
}
