#include "ActorComponents/EquipmentHandlerComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

UEquipmentHandlerComponent::UEquipmentHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EquipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Equipment Mesh"));
}

void UEquipmentHandlerComponent::OnRegister()
{
	Super::OnRegister();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		EquipMeshComponent->SetupAttachment(Character->GetMesh());
	}
}

UEquippableItemInstance* UEquipmentHandlerComponent::GetEquippableItemInstance() const
{
	return nullptr;
}

bool UEquipmentHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	return true;
}

UEquippableItemInstance* UEquipmentHandlerComponent::Unassign()
{
	return nullptr;
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

bool UEquipmentHandlerComponent::CanAssign(const UEquippableItemInstance* EquippableItemInstance) const
{
	return true;
}

bool UEquipmentHandlerComponent::CanUnassign() const
{
	return true;
}

bool UEquipmentHandlerComponent::CanEquip() const
{
	return true;
}

bool UEquipmentHandlerComponent::CanUnequip() const
{
	return true;
}

bool UEquipmentHandlerComponent::CanUse() const
{
	return true;
}

void UEquipmentHandlerComponent::AttachToSocket(FName SocketName)
{
	if (!EquipMeshComponent)
	{
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		EquipMeshComponent->AttachToComponent(
			Character->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);
	}
}
