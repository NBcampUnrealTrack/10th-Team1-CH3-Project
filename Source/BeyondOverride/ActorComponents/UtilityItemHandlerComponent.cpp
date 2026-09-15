#include "ActorComponents/UtilityItemHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/UtilityItemDataRow.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/UtilityItemInstance.h"

UUtilityItemHandlerComponent::UUtilityItemHandlerComponent()
{
	UtilityItemInstance = nullptr;
}

bool UUtilityItemHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Utility Item 인스턴스 저장
	UtilityItemInstance = Cast<UUtilityItemInstance>(EquippableItemInstance);

	// 등록 성공
	return true;
}

UEquippableItemInstance* UUtilityItemHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Utility Item 인스턴스 제거
	UtilityItemInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UUtilityItemHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::Use()
{
	if (!CanUse())
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(UUtilityItemInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UUtilityItemHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UUtilityItemHandlerComponent::CanUnequip() const
{
	if (!Super::CanUnequip())
	{
		return false;
	}

	// 사용 중
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(UseTimerHandle))
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FUtilityItemDataRow* UtilityItemData = UtilityItemInstance->GetUtilityItemData();
	if (!UtilityItemData)
	{
		return false;
	}

	return true;
}
