#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
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
	// 이미 등록된 장비 존재
	if (MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Assign 실패 - %s 장비가 이미 등록됨"), *GetNameSafe(MeleeWeaponInstance))
		return false;
	}

	// 잘못된 아이템 장착 시도
	MeleeWeaponInstance = Cast<UMeleeWeaponInstance>(EquippableItemInstance);
	if (!MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Assign 실패 - %s: UMeleeWeaponInstance가 아님"), *GetNameSafe(EquippableItemInstance))
		return false;
	}

	// 장비 메시 설정
	if (EquipMeshComponent)
	{
		if (USkeletalMesh* Mesh = MeleeWeaponInstance->GetEquippableItemData()->EquipMesh)
		{
			EquipMeshComponent->SetSkeletalMesh(Mesh);
		}
	}

	// 등록 성공
	return true;
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
