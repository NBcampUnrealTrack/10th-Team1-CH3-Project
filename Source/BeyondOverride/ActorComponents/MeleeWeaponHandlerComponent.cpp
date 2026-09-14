#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "GameFramework/Character.h"
#include "Items/Objects/MeleeWeaponInstance.h"

UMeleeWeaponHandlerComponent::UMeleeWeaponHandlerComponent()
{
	MeleeWeaponInstance = nullptr;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::GetEquippableItemInstance() const
{
	return MeleeWeaponInstance;
}

bool UMeleeWeaponHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Melee Weapon 인스턴스 저장
	MeleeWeaponInstance = Cast<UMeleeWeaponInstance>(EquippableItemInstance);

	// 등록 성공
	return true;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Melee Weapon 인스턴스 제거
	MeleeWeaponInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UMeleeWeaponHandlerComponent::Equip()
{
	if (!Equip())
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Use()
{
	if (!Super::Use())
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UMeleeWeaponHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UMeleeWeaponHandlerComponent::CanUnequip() const
{
	return Super::CanUnequip();
}

bool UMeleeWeaponHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FMeleeWeaponDataRow* RangeWeaponData = MeleeWeaponInstance->GetMeleeWeaponData();
	if (!RangeWeaponData)
	{
		return false;
	}

	return true;
}
