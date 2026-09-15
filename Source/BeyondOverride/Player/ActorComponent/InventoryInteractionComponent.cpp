#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "DataTables/Items/ItemDataRow.h"
#include "Factory/ItemFactory.h"
#include "Items/Actors/ItemPickupBase.h"

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

bool UInventoryInteractionComponent::HandleEquipmentSlotClick(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot, bool bLeftClick)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!Inventory->IsValidEquipmentSlot(Slot))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

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
			return PickupEquipmentAll(Inventory, Slot);
		}

		return PickupEquipmentHalf(Inventory, Slot);
	}

	// 손에 아이템이 있음 + 장비 슬롯이 비어 있음
	if (!bSlotHasItem)
	{
		if (bLeftClick)
		{
			return PlaceEquipmentAll(Inventory, Slot);
		}

		return PlaceEquipmentOne(Inventory, Slot);
	}

	// 손에 아이템이 있음 + 같은 아이템
	if (IsSameItem(HoldItem, SlotItem))
	{
		if (bLeftClick)
		{
			return MergeEquipmentAll(Inventory, Slot);
		}

		return MergeEquipmentOne(Inventory, Slot);
	}

	// 손에 아이템이 있음 + 장비 슬롯에 아이템이 있음
	return SwapEquipmentItem(Inventory, Slot);
}

bool UInventoryInteractionComponent::HandlePickupSlotClick(AItemPickupBase* ItemPickup, UInventoryComponent* TargetInventory)
{
	return true;
}

bool UInventoryInteractionComponent::DropItem(bool bLeftClick)
{
	if (bLeftClick)
	{
		return DropAll();
	}

	return DropOne();
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

	const int32 HoldCount = StackCount / 2;

	const int32 RemainingCount = StackCount - HoldCount;

	UItemInstanceBase* NewItem = CreateItemInstance(SlotItem);

	if (!IsValid(NewItem))
	{
		return false;
	}

	NewItem->SetStackCount(HoldCount);
	if (!Inventory->SetItemStackCount(SlotIndex, RemainingCount))
	{
		return false;
	}

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

	const int32 HoldCount = HoldItem->GetStackCount();

	if (HoldCount <= 0)
	{
		return false;
	}

	if (HoldCount == 1)
	{
		return PlaceAll(Inventory, SlotIndex);
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

	const FItemDataRow* ItemData = SlotItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	const int32 MaxStackCount = ItemData->MaxStackCount;
	const int32 CurrentCount = SlotItem->GetStackCount();
	const int32 HoldCount = HoldItem->GetStackCount();
	const int32 Space = MaxStackCount - CurrentCount;

	if (Space <= 0)
	{
		return false;
	}

	const int32 MoveCount = FMath::Min(Space, HoldCount);
	const int32 NewSlotCount = CurrentCount + MoveCount;
	const int32 NewHoldCount = HoldCount - MoveCount;

	if (!Inventory->SetItemStackCount(SlotIndex, NewSlotCount))
	{
		return false;
	}

	HoldItem->SetStackCount(NewHoldCount);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

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

	const FItemDataRow* ItemData = SlotItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	const int32 MaxStackCount = ItemData->MaxStackCount;

	if (SlotItem->GetStackCount() >= MaxStackCount)
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	if (!Inventory->SetItemStackCount(SlotIndex, SlotItem->GetStackCount() + 1))
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

bool UInventoryInteractionComponent::PickupEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	HoldItem = SlotItem;

	if (!Inventory->SetEquipmentItem(Slot, nullptr))
	{
		HoldItem = nullptr;
		return false;
	}

	OnHoldItemChanged.Broadcast(HoldItem);



	return true;
}

bool UInventoryInteractionComponent::PickupEquipmentHalf(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	const int32 StackCount = SlotItem->GetStackCount();

	if (StackCount <= 1)
	{
		return PickupEquipmentAll(Inventory, Slot);
	}

	const int32 HeldCount = StackCount / 2;
	const int32 RemainingCount = StackCount - HeldCount;

	UItemInstanceBase* NewItem = CreateItemInstance(SlotItem);

	if (!IsValid(NewItem))
	{
		return false;
	}

	NewItem->SetStackCount(HeldCount);
	if (!Inventory->SetEquipmentItemStackCount(Slot, RemainingCount))
	{
		return false;
	};

	HoldItem = NewItem;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PlaceEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	if (IsValid(Inventory->GetEquipmentItem(Slot)))
	{
		return false;
	}

	if (!Inventory->SetEquipmentItem(Slot, HoldItem))
	{
		return false;
	}

	HoldItem = nullptr;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PlaceEquipmentOne(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	if (IsValid(Inventory->GetEquipmentItem(Slot)))
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

	if (!Inventory->SetEquipmentItem(Slot, NewItem))
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

bool UInventoryInteractionComponent::MergeEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, SlotItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = SlotItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	const int32 MaxStackCount = ItemData->MaxStackCount;
	const int32 CurrentCount = SlotItem->GetStackCount();
	const int32 HoldCount = HoldItem->GetStackCount();
	const int32 Space = MaxStackCount - CurrentCount;

	if (Space <= 0)
	{
		return false;
	}

	const int32 MoveCount = FMath::Min(Space, HoldCount);
	const int32 NewSlotCount = CurrentCount + MoveCount;
	const int32 NewHoldCount = HoldCount - MoveCount;

	if (!Inventory->SetEquipmentItemStackCount(Slot, NewSlotCount))
	{
		return false;
	}

	HoldItem->SetStackCount(NewHoldCount);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::MergeEquipmentOne(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, SlotItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = SlotItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	if (SlotItem->GetStackCount() >= ItemData->MaxStackCount)
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	if (!Inventory->SetEquipmentItemStackCount(Slot, SlotItem->GetStackCount() + 1))
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

bool UInventoryInteractionComponent::SwapEquipmentItem(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot)
{
	if (!IsValid(Inventory))
	{
		return false;
	}

	if (!IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* SlotItem = Inventory->GetEquipmentItem(Slot);

	if (!IsValid(SlotItem))
	{
		return false;
	}

	if (!Inventory->SetEquipmentItem(Slot, HoldItem))
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

	return FirstItem->GetItemID() == SecondItem->GetItemID();
}

UItemInstanceBase* UInventoryInteractionComponent::CreateItemInstance(UItemInstanceBase* ItemInstance)
{
	if (!IsValid(ItemInstance))
	{
		return nullptr;
	}

	return FItemFactory::CreateItemInstance(this, ItemInstance->GetItemID(), ItemInstance->GetStackCount());
}

bool UInventoryInteractionComponent::DropAll()
{
	if (!IsValid(HoldItem))
	{
		return false;
	}

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	const FVector DropLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
	const FRotator DropRotation = FRotator::ZeroRotator;

	AItemPickupBase* ItemPickup = FItemFactory::SpawnItemPickup(World, HoldItem, DropLocation, DropRotation);

	if (!IsValid(ItemPickup))
	{
		return false;
	}

	HoldItem = nullptr;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::DropOne()
{
	if (!IsValid(HoldItem))
	{
		return false;
	}

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	const FVector DropLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
	const FRotator DropRotation = FRotator::ZeroRotator;

	UItemInstanceBase* DropItem = FItemFactory::CreateItemInstance(this, HoldItem->GetItemID(), 1);

	if (!IsValid(DropItem))
	{
		return false;
	}

	AItemPickupBase* ItemPickup = FItemFactory::SpawnItemPickup(World, DropItem, DropLocation, DropRotation);

	if (!IsValid(ItemPickup))
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
