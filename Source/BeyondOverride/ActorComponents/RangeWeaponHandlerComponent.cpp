#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"

URangeWeaponHandlerComponent::URangeWeaponHandlerComponent()
{
}

UEquippableItemInstance* URangeWeaponHandlerComponent::GetEquippableItemInstance() const
{
	return RangeWeaponInstance;
}

bool URangeWeaponHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	// 이미 등록된 장비 존재
	if (RangeWeaponInstance)
	{
		return false;
	}

	// 잘못된 아이템 장착 시도
	RangeWeaponInstance = Cast<URangeWeaponInstance>(EquippableItemInstance);
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 등록 성공
	return true;
}

bool URangeWeaponHandlerComponent::Unassign()
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 장착 해제 시도
	if (!Unequip())
	{
		return false;
	}

	// 장비 제거
	RangeWeaponInstance = nullptr;

	// 제거 성공
	return true;
}

bool URangeWeaponHandlerComponent::Equip()
{
	return true;
}

bool URangeWeaponHandlerComponent::Unequip()
{
	return true;
}

bool URangeWeaponHandlerComponent::Use()
{
	return true;
}
