#include "ActorComponents/ThrowableItemHandlerComponent.h"

#include "DataTables/Items/ThrowableItemDataRow.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/ThrowableItemInstance.h"

UThrowableItemHandlerComponent::UThrowableItemHandlerComponent()
{
	ThrowableItemInstance = nullptr;
}

bool UThrowableItemHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Throwable Item 인스턴스 저장
	ThrowableItemInstance = Cast<UThrowableItemInstance>(EquippableItemInstance);

	// 등록 성공
	return true;
}

UEquippableItemInstance* UThrowableItemHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Throwable Item 인스턴스 제거
	ThrowableItemInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UThrowableItemHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::Use()
{
	return false;
}

bool UThrowableItemHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	return false;
}

bool UThrowableItemHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UThrowableItemHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UThrowableItemHandlerComponent::CanUnequip() const
{
	return Super::CanUnequip();
}

bool UThrowableItemHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FThrowableItemDataRow* ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();
	if (!ThrowableItemData)
	{
		return false;
	}

	return true;
}
