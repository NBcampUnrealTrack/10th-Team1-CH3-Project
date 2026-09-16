#include "Player/ActorComponent/PlayerInventoryComponent.h"

#include "Enums/EquipmentSlot.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeEquipmentSlot();
}

void UPlayerInventoryComponent::InitializeEquipmentSlot()
{
	EquipmentSlots.Init(nullptr, 7);
}

bool UPlayerInventoryComponent::CanEquipItem(EEquipmentSlot Slot, const UItemInstanceBase* Item) const
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

	switch (Slot)
	{
	case EEquipmentSlot::Primary:
	case EEquipmentSlot::Secondary:
		return ItemData->ItemType == EItemType::RangeWeapon;

	case EEquipmentSlot::Melee:
		return ItemData->ItemType == EItemType::MeleeWeapon;

	case EEquipmentSlot::Throwable:
		return ItemData->ItemType == EItemType::ThrowableItem;

	case EEquipmentSlot::Effect:
		return ItemData->ItemType == EItemType::EffectItem;

	case EEquipmentSlot::Bag:
		return ItemData->ItemType == EItemType::BagItem;

	case EEquipmentSlot::Shield:
		return ItemData->ItemType == EItemType::ShieldItem;

	default:
		return false;
	}
}

bool UPlayerInventoryComponent::SetEquipmentItemStackCount(EEquipmentSlot Slot, int32 StackCount)
{
	const int32 SlotIndex = GetEquipmentSlotIndex(Slot);

	if (!EquipmentSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	UItemInstanceBase* Item = EquipmentSlots[SlotIndex];

	if (!IsValid(Item))
	{
		return false;
	}

	if (StackCount <= 0)
	{
		EquipmentSlots[SlotIndex] = nullptr;

		OnEquipmentItemChanged.Broadcast(Slot, nullptr);
		OnEquipmentSlotChanged.Broadcast(Slot, nullptr);

		return true;
	}

	Item->SetStackCount(StackCount);

	OnEquipmentSlotChanged.Broadcast(Slot, Item);

	return true;
}

int32 UPlayerInventoryComponent::GetEquipmentSlotIndex(EEquipmentSlot Slot) const
{
	return static_cast<int32>(Slot);
}

bool UPlayerInventoryComponent::IsValidEquipmentSlot(EEquipmentSlot Slot) const
{
	return EquipmentSlots.IsValidIndex(GetEquipmentSlotIndex(Slot));
}

UItemInstanceBase* UPlayerInventoryComponent::GetEquipmentItem(EEquipmentSlot Slot) const
{
	const int32 SlotIndex = GetEquipmentSlotIndex(Slot);

	if (!EquipmentSlots.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	return EquipmentSlots[SlotIndex];
}

bool UPlayerInventoryComponent::SetEquipmentSlots(const TArray<UItemInstanceBase*>& NewSlots)
{
	if (NewSlots.Num() > EquipmentSlots.Num())
	{
		return false;
	}

	const TArray<TObjectPtr<UItemInstanceBase>> PreviousSlots = EquipmentSlots;

	EquipmentSlots.Init(nullptr, 7);

	for (int32 i = 0; i < NewSlots.Num(); ++i)
	{
		EquipmentSlots[i] = DuplicateObject<UItemInstanceBase>(NewSlots[i], this);

		if (IsValid(NewSlots[i]))
		{
			EquipmentSlots[i]->Initialize();
		}
	}

	for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
	{
		const EEquipmentSlot Slot = static_cast<EEquipmentSlot>(i);

		UItemInstanceBase* PreviousItem = PreviousSlots.IsValidIndex(i) ? PreviousSlots[i].Get() : nullptr;
		UItemInstanceBase* NewItem = EquipmentSlots[i];

		if (PreviousItem != NewItem)
		{
			OnEquipmentItemChanged.Broadcast(Slot, NewItem);
		}

		OnEquipmentSlotChanged.Broadcast(Slot, NewItem);
	}

	return true;
}

bool UPlayerInventoryComponent::SetEquipmentItem(EEquipmentSlot Slot, UItemInstanceBase* Item)
{
	const int32 SlotIndex = GetEquipmentSlotIndex(Slot);

	if (!EquipmentSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	if (IsValid(Item) && !CanEquipItem(Slot, Item))
	{
		return false;
	}

	UItemInstanceBase* PreviousItem = EquipmentSlots[SlotIndex];

	EquipmentSlots[SlotIndex] = Item;

	// 아이템 자체가 달라졌을 때만 장비 동기화 이벤트 호출
	if (PreviousItem != Item)
	{
		OnEquipmentItemChanged.Broadcast(Slot, Item);
	}

	// UI는 항상 갱신
	OnEquipmentSlotChanged.Broadcast(Slot, Item);

	return true;
}
