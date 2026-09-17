#include "Player/ActorComponent/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlot();
}

void UInventoryComponent::InitializeSlot()
{
	if (MaxSlotCount <= 0)
	{
		return;
	}

	Slots.Init(nullptr, MaxSlotCount);

	OnInventoryChanged.Broadcast(Slots);
}

bool UInventoryComponent::AddItem(UItemInstanceBase* Item, const int32 SlotIndex)
{
	if (!IsValid(Item))
	{
		return false;
	}

	const FItemDataRow* ItemData = Item->GetItemData();

	if (!ItemData)
	{
		return false;
	}

	if (SlotIndex != INDEX_NONE)
	{
		if (!Slots.IsValidIndex(SlotIndex))
		{
			return false;
		}

		if (IsValid(Slots[SlotIndex]))
		{
			return false;
		}

		Slots[SlotIndex] = Item;

		OnInventoryChanged.Broadcast(Slots);

		return true;
	}

	const FName ItemID = Item->GetItemID();
	const int32 MaxStackCount = ItemData->MaxStackCount;
	int32 RemainingCount = Item->GetStackCount();

	int32 AvailableCount = 0;
	bool bHasEmptySlot = false;

	for (UItemInstanceBase* SlotItem : Slots)
	{
		if (!IsValid(SlotItem))
		{
			bHasEmptySlot = true;
			continue;
		}

		if (SlotItem->GetItemID() == ItemID)
		{
			AvailableCount += MaxStackCount - SlotItem->GetStackCount();
		}
	}

	if (bHasEmptySlot)
	{
		AvailableCount += MaxStackCount;
	}

	if (AvailableCount < RemainingCount)
	{
		return false;
	}

	// 같은 아이템이 들어 있는 슬롯들을 먼저 채우기
	for (UItemInstanceBase* SlotItem : Slots)
	{
		if (!IsValid(SlotItem))
		{
			continue;
		}

		if (SlotItem->GetItemID() != ItemID)
		{
			continue;
		}

		const int32 AddableCount = MaxStackCount - SlotItem->GetStackCount();

		if (AddableCount <= 0)
		{
			continue;
		}

		const int32 AddCount = FMath::Min(RemainingCount, AddableCount);

		SlotItem->SetStackCount(SlotItem->GetStackCount() + AddCount);

		RemainingCount -= AddCount;

		if (RemainingCount <= 0)
		{
			break;
		}
	}

	// 합치고도 남았다면 빈 슬롯에 넣기
	if (RemainingCount > 0)
	{
		int32 EmptySlotIndex = INDEX_NONE;

		if (!FindEmptySlotIndex(EmptySlotIndex))
		{
			return false;
		}

		Item->SetStackCount(RemainingCount);
		Slots[EmptySlotIndex] = Item;
	}

	OnInventoryChanged.Broadcast(Slots);

	return true;

}

bool UInventoryComponent::RemoveItem(const int32 SlotIndex, const int32 Count)
{
	if (Count <= 0)
	{
		return false;
	}

	if (!Slots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	UItemInstanceBase* Item = Slots[SlotIndex];

	if (!IsValid(Item))
	{
		return false;
	}

	const int32 NewCount = Item->GetStackCount() - Count;

	if (NewCount <= 0)
	{
		Slots[SlotIndex] = nullptr;
	}
	else
	{
		Item->SetStackCount(NewCount);
	}

	OnInventoryChanged.Broadcast(Slots);

	return true;
}

bool UInventoryComponent::SwapSlots(const int32 FirstIndex, const int32 SecondIndex)
{
	if (!Slots.IsValidIndex(FirstIndex) || !Slots.IsValidIndex(SecondIndex))
	{
		return false;
	}

	Swap(Slots[FirstIndex], Slots[SecondIndex]);

	OnInventoryChanged.Broadcast(Slots);

	return true;
}

TArray<UItemInstanceBase*> UInventoryComponent::GetSlots() const
{
	return Slots;
}

UItemInstanceBase* UInventoryComponent::GetItem(const int32 SlotIndex) const
{
	if (!Slots.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	return Slots[SlotIndex];
}

bool UInventoryComponent::SetSlots(const TArray<UItemInstanceBase*>& NewSlots)
{
	if (NewSlots.Num() > Slots.Num())
	{
		return false;
	}

	Slots.Init(nullptr, MaxSlotCount);

	for (int32 i = 0; i < NewSlots.Num(); ++i)
	{
		Slots[i] = DuplicateObject<UItemInstanceBase>(NewSlots[i], this);

		if (IsValid(NewSlots[i]))
		{
			Slots[i]->Initialize();
		}
	}

	OnInventoryChanged.Broadcast(Slots);

	return true;
}

bool UInventoryComponent::SetItem(const int32 SlotIndex, UItemInstanceBase* Item)
{
	if (!Slots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	Slots[SlotIndex] = Item;

	OnInventoryChanged.Broadcast(Slots);

	return true;
}

bool UInventoryComponent::SetItemStackCount(int32 SlotIndex, int32 StackCount)
{
	if (!Slots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	UItemInstanceBase* Item = Slots[SlotIndex];

	if (!IsValid(Item))
	{
		return false;
	}

	if (StackCount <= 0)
	{
		Slots[SlotIndex] = nullptr;
	}
	else
	{
		Item->SetStackCount(StackCount);
	}

	OnInventoryChanged.Broadcast(Slots);

	return true;
}

int32 UInventoryComponent::GetSlotCount() const
{
	return Slots.Num();
}

bool UInventoryComponent::IsValidSlot(const int32 SlotIndex) const
{
	return Slots.IsValidIndex(SlotIndex);
}

bool UInventoryComponent::FindEmptySlotIndex(int32& EmptySlotIndex) const
{
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (!IsValid(Slots[i]))
		{
			EmptySlotIndex = i;
			return true;
		}
	}

	return false;
}

int32 UInventoryComponent::FindItemIndex(const FName& ItemID) const
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (IsValid(Slots[i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Id : "), *Slots[i]->GetItemID().ToString());
			if (ItemID == Slots[i]->GetItemID())
			{
				return i;
			}
		}
	}

	return INDEX_NONE;
}
