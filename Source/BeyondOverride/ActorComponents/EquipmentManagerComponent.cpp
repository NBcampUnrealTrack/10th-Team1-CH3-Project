#include "ActorComponents/EquipmentManagerComponent.h"

#include "Enums/EquipmentSlot.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveSlot = EEquipmentSlot::Primary;
}

void UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
}

void UEquipmentManagerComponent::Unequip()
{
}

void UEquipmentManagerComponent::Use()
{
}

void UEquipmentManagerComponent::Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase)
{
}

void UEquipmentManagerComponent::Unassign(EEquipmentSlot Slot)
{
}
