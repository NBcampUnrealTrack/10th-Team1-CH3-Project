#include "ActorComponents/EquipmentHandlerComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "GameFramework/Character.h"
#include "Items/Objects/EquippableItemInstance.h"

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

bool UEquipmentHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 장비 인스턴스 저장
	EquippableItemInstance = InEquippableItemInstance;

	// 장비 메시 설정
	if (EquipMeshComponent)
	{
		const FEquippableItemDataRow* EquippableItemData = InEquippableItemInstance->GetEquippableItemData(); // 장비 데이터

		if (USkeletalMesh* Mesh = EquippableItemData->EquipMesh)
		{
			EquipMeshComponent->SetSkeletalMesh(Mesh);
		}
	}

	return true;
}

UEquippableItemInstance* UEquipmentHandlerComponent::Unassign()
{
	if (!CanUnassign())
	{
		return nullptr;
	}

	// 장비 해제 실패
	if (!Unequip())
	{
		return nullptr;
	}

	// 장비 메시 제거
	if (EquipMeshComponent)
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

	// 장비 데이터 제거
	UEquippableItemInstance* OutEquippableItemInstance = EquippableItemInstance;
	EquippableItemInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
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

bool UEquipmentHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	// 이미 등록된 장비 데이터 존재
	if (EquippableItemInstance)
	{
		return false;
	}

	// 유효하지 않은 EquippableItemInstance
	if (!InEquippableItemInstance)
	{
		return false;
	}

	// 유효하지 않은 EquippableItemData
	if (!EquippableItemInstance->GetEquippableItemData())
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUnassign() const
{
	// 등록된 장비 없음
	if (!EquippableItemInstance)
	{
		return false;
	}

	// 장비 해제 불가
	if (!CanUnequip())
	{
		return false;
	}

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
