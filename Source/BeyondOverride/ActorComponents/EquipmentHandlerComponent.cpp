#include "ActorComponents/EquipmentHandlerComponent.h"

UEquipmentHandlerComponent::UEquipmentHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEquipmentHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	return true;
}

bool UEquipmentHandlerComponent::Unassign()
{
	return true;
}

bool UEquipmentHandlerComponent::Equip()
{
	return true;
}

bool UEquipmentHandlerComponent::Unequip()
{
	return true;
}

bool UEquipmentHandlerComponent::Use()
{
	return true;
}
