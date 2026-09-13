#include "ActorComponents/EquipmentHandlerComponent.h"

#include "Components/SkeletalMeshComponent.h"

UEquipmentHandlerComponent::UEquipmentHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EquipMeshComponent = nullptr;
}

void UEquipmentHandlerComponent::OnRegister()
{
	EquipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Equipment Mesh"));
}

UEquippableItemInstance* UEquipmentHandlerComponent::GetEquippableItemInstance() const
{
	return nullptr;
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
