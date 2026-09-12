#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"

URangeWeaponHandlerComponent::URangeWeaponHandlerComponent()
{
	// 틱 가능 & 초기 비활성화
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

	// 틱 활성화
	SetComponentTickEnabled(true);

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

	// 틱 비활성화
	SetComponentTickEnabled(false);

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

void URangeWeaponHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
