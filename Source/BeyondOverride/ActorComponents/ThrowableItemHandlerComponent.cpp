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
	if (!CanThrow())
	{
		return false;
	}

	// TODO: 투척 로직 구현

	// 투척 타이머 활성화
	StartThrowTimer();

	return true;
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
	if (!Super::CanUnequip())
	{
		return false;
	}

	// 투척 중
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ThrowTimerHandle))
	{
		return false;
	}

	return true;
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

bool UThrowableItemHandlerComponent::CanThrow() const
{
	if (!CanUse())
	{
		return false;
	}

	// 투척 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(ThrowTimerHandle))
	{
		return false;
	}

	return true;
}

void UThrowableItemHandlerComponent::StartThrowTimer()
{
	// 데이터 유효성 검증
	const FThrowableItemDataRow* ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();
	if (!ThrowableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UThrowableItemHandlerComponent] 투척 타이머 활성화 실패 - 유효하지 않은 ThrowableItemData"));
		return;
	}

	// 공격 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		ThrowTimerHandle,
		ThrowableItemData->ThrowDuration,
		false);
}
