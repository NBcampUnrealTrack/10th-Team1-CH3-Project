#include "Player/ActorComponent/InventoryInteractionComponent.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Player/ActorComponent/InventoryComponent.h"

UInventoryInteractionComponent::UInventoryInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryInteractionComponent::HandleSlotClick(UInventoryComponent* Inventory, const int32 SlotIndex, const bool bLeftClick)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!Inventory->IsValidSlot(SlotIndex))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	const bool bHoldingItem = IsValid(HoldItem);
	const bool bSlotHasItem = IsValid(SlotItem);

	// 손에 아무것도 없음
	if (!bHoldingItem)
	{
		if (!bSlotHasItem)
		{
			return false;
		}

		if (bLeftClick)
		{
			return PickupAll(Inventory, SlotIndex);
		}

		return PickupHalf(Inventory, SlotIndex);
	}

	// 손에 아이템이 있음 + 슬롯이 비어 있음
	if (!bSlotHasItem)
	{
		if (bLeftClick)
		{
			return PlaceAll(Inventory, SlotIndex);
		}

		return PlaceOne(Inventory, SlotIndex);
	}

	// 손에 아이템이 있음 + 슬롯에도 같은 아이템이 있음
	if (IsSameItem(HoldItem, SlotItem))
	{
		if (bLeftClick)
		{
			return MergeAll(Inventory, SlotIndex);
		}

		return MergeOne(Inventory, SlotIndex);
	}

	// 손에 아이템이 있음 + 슬롯에는 다른 아이템이 있음
	return SwapHeldItem(Inventory, SlotIndex);
}

bool UInventoryInteractionComponent::IsHoldingItem() const
{
	return IsValid(HoldItem);
}

UItemInstanceBase* UInventoryInteractionComponent::GetHoldItem() const
{
	return HoldItem;
}

bool UInventoryInteractionComponent::PickupAll(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	const int32 StackCount = SlotItem->GetStackCount();

	HoldItem = SlotItem;

	if (!Inventory->RemoveItem(SlotIndex, StackCount))
	{
		HoldItem = nullptr;

		return false;
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PickupHalf(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	const int32 StackCount = SlotItem->GetStackCount();

	if (StackCount <= 1)
	{
		return PickupAll(
			Inventory,
			SlotIndex);
	}

	const int32 HeldCount = StackCount / 2;

	const int32 RemainingCount = StackCount - HeldCount;

	UItemInstanceBase* NewItem = CreateItemInstance(SlotItem);

	if (!IsValid(NewItem))
	{
		return false;
	}

	NewItem->SetStackCount(HeldCount);
	SlotItem->SetStackCount(RemainingCount);

	Inventory->NotifyInventoryChanged();

	HoldItem = NewItem;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PlaceAll(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	if (IsValid(Inventory->GetItem(SlotIndex)))
	{
		return false;
	}

	if (!Inventory->AddItem(HoldItem, SlotIndex))
	{
		return false;
	}

	HoldItem = nullptr;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PlaceOne(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	if (IsValid(Inventory->GetItem(SlotIndex)))
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	UItemInstanceBase* NewItem = CreateItemInstance(HoldItem);

	if (!IsValid(NewItem))
	{
		return false;
	}

	NewItem->SetStackCount(1);

	if (!Inventory->AddItem(NewItem, SlotIndex))
	{
		return false;
	}

	HoldItem->SetStackCount(HoldItem->GetStackCount() - 1);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::MergeAll(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, SlotItem))
	{
		return false;
	}

	const int32 MaxStackCount = SlotItem->GetItemData()->MaxStackCount;
	const int32 CurrentCount = SlotItem->GetStackCount();
	const int32 HoldCount = HoldItem->GetStackCount();
	const int32 Space = MaxStackCount - CurrentCount;

	if (Space <= 0)
	{
		return false;
	}

	const int32 MoveCount = FMath::Min(Space, HoldCount);

	SlotItem->SetStackCount(CurrentCount + MoveCount);
	HoldItem->SetStackCount(HoldCount - MoveCount);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	Inventory->NotifyInventoryChanged();

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::MergeOne(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, SlotItem))
	{
		return false;
	}

	const int32 MaxStackCount = SlotItem->GetItemData()->MaxStackCount;

	if (SlotItem->GetStackCount() >= MaxStackCount)
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	SlotItem->SetStackCount(SlotItem->GetStackCount() + 1);

	HoldItem->SetStackCount(HoldItem->GetStackCount() - 1);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	Inventory->NotifyInventoryChanged();

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::SwapHeldItem(UInventoryComponent* Inventory, const int32 SlotIndex)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetItem(SlotIndex);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!Inventory->SetItem(SlotIndex, HoldItem))
	{
		return false;
	}

	HoldItem = SlotItem;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::IsSameItem(const UItemInstanceBase* FirstItem, const UItemInstanceBase* SecondItem) const
{
	if (!IsValid(FirstItem) || !IsValid(SecondItem))
	{
		return false;
	}

	// 이 부분은 변경이 필요함. 두 아이템이 같은 종류인지 검사를 어떻게 하는가?
	return FirstItem->GetItemData() == SecondItem->GetItemData();
}

UItemInstanceBase* UInventoryInteractionComponent::CreateItemInstance(UItemInstanceBase* ItemInstance)
{
	// 아이템 생성 방식은 아직 결정하지 않았으므로 임시 구현
	return nullptr;
}
