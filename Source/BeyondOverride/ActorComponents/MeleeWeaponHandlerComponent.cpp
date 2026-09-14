#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "Items/Objects/MeleeWeaponInstance.h"

UMeleeWeaponHandlerComponent::UMeleeWeaponHandlerComponent()
{
	MeleeWeaponInstance = nullptr;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::GetEquippableItemInstance() const
{
	return MeleeWeaponInstance;
}

bool UMeleeWeaponHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	return false;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::Unassign()
{
	return nullptr;
}

bool UMeleeWeaponHandlerComponent::Equip()
{
	return false;
}

bool UMeleeWeaponHandlerComponent::Unequip()
{
	return false;
}

bool UMeleeWeaponHandlerComponent::Use()
{
	return false;
}

bool UMeleeWeaponHandlerComponent::CanUnequip()
{
	return false;
}
