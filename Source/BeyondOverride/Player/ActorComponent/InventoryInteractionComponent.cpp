#include "Player/ActorComponent/InventoryInteractionComponent.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "DataTables/Items/ItemDataRow.h"
#include "Factory/ItemFactory.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Player/ActorComponent/NearbyItemComponent.h"

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

	// 손에 아이템이 있음 + 장비 슬롯에도 같은 아이템이 있음
	if (IsSameItem(HoldItem, SlotItem))
	{
		if (bLeftClick)
		{
			return MergeEquipmentAll(Inventory, Slot);
		}

		return MergeEquipmentOne(Inventory, Slot);
	}

	// 손에 아이템이 있음 + 장비 슬롯에는 다른 아이템이 있음
	return SwapEquipmentItem(Inventory, Slot);
}

bool UInventoryInteractionComponent::HandleNearbySlotClick(UNearbyItemComponent* NearbyItemComponent, int32 SlotIndex, bool bLeftClick)
{
	if (!IsValid(NearbyItemComponent))
	{
		return false;
	}

	const int32 ItemCount = NearbyItemComponent->GetItemCount();

	// 0 ~ ItemCount - 1: 실제 바닥 아이템
	// ItemCount: 새 아이템을 버리는 빈 슬롯
	if (SlotIndex < 0 || SlotIndex > ItemCount)
	{
		return false;
	}

	AItemPickupBase* ItemPickup = nullptr;

	if (SlotIndex < ItemCount)
	{
		ItemPickup = NearbyItemComponent->GetItemPickup(SlotIndex);
	}

	UItemInstanceBase* PickupItem = nullptr;

	if (IsValid(ItemPickup))
	{
		PickupItem = ItemPickup->GetItemInstance();
	}

	const bool bHoldingItem = IsValid(HoldItem);
	const bool bSlotHasItem = IsValid(PickupItem);

	// 손에 아무 것도 없음
	if (!bHoldingItem)
	{
		if (!bSlotHasItem)
		{
			return false;
		}

		if (bLeftClick)
		{
			return PickupWorldAll(NearbyItemComponent, ItemPickup);
		}

		return PickupWorldHalf(NearbyItemComponent, ItemPickup);
	}

	// 손에 아이템이 있음 + 외부 슬롯이 비어 있음
	if (!bSlotHasItem)
	{
		if (bLeftClick)
		{
			return DropAll(NearbyItemComponent);
		}

		return DropOne(NearbyItemComponent);
	}

	// 손에 아이템이 있음 + 외부 슬롯에도 같은 아이템이 있음
	if (IsSameItem(HoldItem, PickupItem))
	{
		if (bLeftClick)
		{
			return MergeWorldAll(NearbyItemComponent, ItemPickup);
		}

		return MergeWorldOne(NearbyItemComponent, ItemPickup);
	}

	// 손에 아이템이 있음 + 외부 슬롯에는 다른 아이템이 있음
	return SwapWorldItem(NearbyItemComponent, ItemPickup);
}

bool UInventoryInteractionComponent::DropItem(bool bLeftClick)
{
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UNearbyItemComponent* NearbyItemComponent = Owner->FindComponentByClass<UNearbyItemComponent>();

	if (!IsValid(NearbyItemComponent))
	{
		return false;
	}

	if (bLeftClick)
	{
		return DropAll(NearbyItemComponent);
	}

	return DropOne(NearbyItemComponent);
}

bool UInventoryInteractionComponent::SellItem(bool bLeftClick)
{
	if (!IsValid(HoldItem))
	{
		return false;
	}

	if (bLeftClick)
	{
		return SellAll();
	}

	return SellOne();
}

bool UInventoryInteractionComponent::BuyItem(UItemInstanceBase* Item)
{
	if (!IsValid(Item))
	{
		return false;
	}

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UPlayerInventoryComponent* PlayerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();

	if (!IsValid(PlayerInventory))
	{
		return false;
	}

	const FItemDataRow* ItemData = Item->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	const int32 ItemCount = Item->GetStackCount();

	if (ItemCount <= 0 || ItemData->BuyPrice < 0)
	{
		return false;
	}

	const int64 Price = ItemData->BuyPrice * ItemCount;

	if (PlayerInventory->GetMoney() < Price)
	{
		return false;
	}

	if (!PlayerInventory->AddItem(Item))
	{
		return false;
	}

	if (Price == 0)
	{
		return true;
	}

	return PlayerInventory->SpendMoney(Price);
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
		return PickupAll(Inventory, SlotIndex);
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

bool UInventoryInteractionComponent::PickupWorldAll(UNearbyItemComponent* NearbyItemComponent, AItemPickupBase* ItemPickup)
{
	if (!IsValid(NearbyItemComponent) || !IsValid(ItemPickup))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* PickupItem = ItemPickup->GetItemInstance();

	if (!IsValid(PickupItem))
	{
		return false;
	}

	HoldItem = PickupItem;

	NearbyItemComponent->RemoveItemPickup(ItemPickup);

	ItemPickup->Destroy();

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::PickupWorldHalf(UNearbyItemComponent* NearbyItemComponent, AItemPickupBase* ItemPickup)
{
	if (!IsValid(NearbyItemComponent) || !IsValid(ItemPickup))
	{
		return false;
	}

	if (IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* PickupItem = ItemPickup->GetItemInstance();

	if (!IsValid(PickupItem))
	{
		return false;
	}

	const int32 StackCount = PickupItem->GetStackCount();

	if (StackCount <= 1)
	{
		return PickupWorldAll(NearbyItemComponent, ItemPickup);
	}

	const int32 HoldCount = StackCount / 2;
	const int32 RemainingCount = StackCount - HoldCount;

	UItemInstanceBase* NewItem = CreateItemInstance(PickupItem);

	if (!IsValid(NewItem))
	{
		return false;
	}

	NewItem->SetStackCount(HoldCount);
	PickupItem->SetStackCount(RemainingCount);

	HoldItem = NewItem;

	OnHoldItemChanged.Broadcast(HoldItem);

	NearbyItemComponent->NotifyItemsChanged();

	return true;
}

bool UInventoryInteractionComponent::MergeWorldAll(UNearbyItemComponent* NearbyItemComponent, AItemPickupBase* ItemPickup)
{
	if (!IsValid(NearbyItemComponent) || !IsValid(ItemPickup) || !IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* PickupItem = ItemPickup->GetItemInstance();

	if (!IsValid(PickupItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, PickupItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = PickupItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	const int32 MaxStackCount = ItemData->MaxStackCount;
	const int32 CurrentCount = PickupItem->GetStackCount();
	const int32 HoldCount = HoldItem->GetStackCount();
	const int32 Space = MaxStackCount - CurrentCount;

	if (Space <= 0)
	{
		return false;
	}

	const int32 MoveCount = FMath::Min(Space, HoldCount);

	PickupItem->SetStackCount(CurrentCount + MoveCount);
	HoldItem->SetStackCount(HoldCount - MoveCount);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	NearbyItemComponent->NotifyItemsChanged();

	return true;
}

bool UInventoryInteractionComponent::MergeWorldOne(UNearbyItemComponent* NearbyItemComponent, AItemPickupBase* ItemPickup)
{
	if (!IsValid(NearbyItemComponent) || !IsValid(ItemPickup) || !IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* PickupItem = ItemPickup->GetItemInstance();

	if (!IsValid(PickupItem))
	{
		return false;
	}

	if (!IsSameItem(HoldItem, PickupItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = PickupItem->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	if (PickupItem->GetStackCount() >= ItemData->MaxStackCount)
	{
		return false;
	}

	if (HoldItem->GetStackCount() <= 0)
	{
		return false;
	}

	PickupItem->SetStackCount(PickupItem->GetStackCount() + 1);
	HoldItem->SetStackCount(HoldItem->GetStackCount() - 1);

	if (HoldItem->GetStackCount() <= 0)
	{
		HoldItem = nullptr;
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	NearbyItemComponent->NotifyItemsChanged();

	return true;
}

bool UInventoryInteractionComponent::SwapWorldItem(UNearbyItemComponent* NearbyItemComponent, AItemPickupBase* ItemPickup)
{
	if (!IsValid(NearbyItemComponent) || !IsValid(ItemPickup) || !IsValid(HoldItem))
	{
		return false;
	}

	UItemInstanceBase* PickupItem = ItemPickup->GetItemInstance();

	if (!IsValid(PickupItem))
	{
		return false;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	const FVector SpawnLocation = ItemPickup->GetActorLocation();
	const FRotator SpawnRotation = ItemPickup->GetActorRotation();

	AItemPickupBase* NewItemPickup = FItemFactory::SpawnItemPickup(World, HoldItem, SpawnLocation, SpawnRotation);

	if (!IsValid(NewItemPickup))
	{
		return false;
	}

	NearbyItemComponent->RemoveItemPickup(ItemPickup);

	ItemPickup->Destroy();

	NearbyItemComponent->AddItemPickup(NewItemPickup);

	HoldItem = PickupItem;

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

bool UInventoryInteractionComponent::DropAll(UNearbyItemComponent* NearbyItemComponent)
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

	if (IsValid(NearbyItemComponent))
	{
		NearbyItemComponent->AddItemPickup(ItemPickup);
	}


	HoldItem = nullptr;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::DropOne(UNearbyItemComponent* NearbyItemComponent)
{
	if (!IsValid(HoldItem))
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
		return DropAll(NearbyItemComponent);
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

	UItemInstanceBase* DropItem = FItemFactory::CreateItemInstance(this, HoldItem->GetItemID(), 1);

	if (!IsValid(DropItem))
	{
		return false;
	}

	const FVector DropLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f;
	const FRotator DropRotation = FRotator::ZeroRotator;

	AItemPickupBase* ItemPickup = FItemFactory::SpawnItemPickup(World, DropItem, DropLocation, DropRotation);

	if (!IsValid(ItemPickup))
	{
		return false;
	}

	if (IsValid(NearbyItemComponent))
	{
		NearbyItemComponent->AddItemPickup(ItemPickup);
	}

	HoldItem->SetStackCount(HoldCount - 1);

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::SellAll()
{
	if (!IsValid(HoldItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = HoldItem->GetItemData();

	if (!ItemData || ItemData->SellPrice <= 0)
	{
		return false;
	}

	const int32 HoldCount = HoldItem->GetStackCount();

	if (HoldCount <= 0)
	{
		return false;
	}

	const int64 TotalPrice =
		static_cast<int64>(ItemData->SellPrice) * HoldCount;

	if (TotalPrice > MAX_int32)
	{
		return false;
	}

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UPlayerInventoryComponent* PlayerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();

	if (!IsValid(PlayerInventory))
	{
		return false;
	}

	if (!PlayerInventory->AddMoney(static_cast<int32>(TotalPrice)))
	{
		return false;
	}

	// 전체 판매 완료
	HoldItem = nullptr;

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}

bool UInventoryInteractionComponent::SellOne()
{
	if (!IsValid(HoldItem))
	{
		return false;
	}

	const FItemDataRow* ItemData = HoldItem->GetItemData();

	if (!ItemData || ItemData->SellPrice <= 0)
	{
		return false;
	}

	const int32 HoldCount = HoldItem->GetStackCount();

	if (HoldCount <= 0)
	{
		return false;
	}

	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return false;
	}

	UPlayerInventoryComponent* PlayerInventory = Owner->FindComponentByClass<UPlayerInventoryComponent>();

	if (!IsValid(PlayerInventory))
	{
		return false;
	}

	if (!PlayerInventory->AddMoney(ItemData->SellPrice))
	{
		return false;
	}

	const int32 NewCount = HoldCount - 1;

	if (NewCount <= 0)
	{
		HoldItem = nullptr;
	}
	else
	{
		HoldItem->SetStackCount(NewCount);
	}

	OnHoldItemChanged.Broadcast(HoldItem);

	return true;
}
