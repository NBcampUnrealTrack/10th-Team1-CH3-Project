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
