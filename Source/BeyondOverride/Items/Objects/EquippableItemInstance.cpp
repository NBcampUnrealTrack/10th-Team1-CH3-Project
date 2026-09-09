#include "Items/Objects/EquippableItemInstance.h"

UEquippableItemInstance::UEquippableItemInstance()
{
	EquippableItemData = nullptr;
}

const UEquippableItemDataAsset* UEquippableItemInstance::GetEquippableItemData() const
{
	return EquippableItemData;
}
