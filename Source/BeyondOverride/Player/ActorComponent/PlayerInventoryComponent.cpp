#include "Player/ActorComponent/PlayerInventoryComponent.h"

#include "Enums/EquipmentSlot.h"
#include "DataTables/Items/BackpackDataRow.h"
#include "Items/Objects/ItemInstanceBase.h"


UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxSlotCount = 10;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	BaseSlotCount = MaxSlotCount;
	BaseMaxCarryWeight = MaxCarryWeight;

	InitializeEquipmentSlot();
}

void UPlayerInventoryComponent::NotifyInventoryChanged()
{
	Super::NotifyInventoryChanged();

	RecalculateCarryWeight();
}

void UPlayerInventoryComponent::RecalculateCarryWeight()
{
	float NewCarryWeight = 0.0f;

	for (const UItemInstanceBase* Item : Slots)
	{
		if (!IsValid(Item))
		{
			continue;
		}

		const FItemDataRow* ItemData = Item->GetItemData();

		if (ItemData == nullptr)
		{
			continue;
		}

		NewCarryWeight += ItemData->Weight * Item->GetStackCount();
	}

	for (const UItemInstanceBase* Item : EquipmentSlots)
	{
		if (!IsValid(Item))
		{
			continue;
		}

		const FItemDataRow* ItemData = Item->GetItemData();

		if (ItemData == nullptr)
		{
			continue;
		}

		NewCarryWeight += ItemData->Weight * Item->GetStackCount();
	}

	CurCarryWeight = NewCarryWeight;

	OnWeightChanged.Broadcast(CurCarryWeight, MaxCarryWeight);
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
	}
	else
	{
		Item->SetStackCount(StackCount);

		OnEquipmentSlotChanged.Broadcast(Slot, Item);
	}

	RecalculateCarryWeight();

	return true;
}

TArray<UItemInstanceBase*> UPlayerInventoryComponent::ApplyBackpack(const FBackpackDataRow* BackpackData)
{
	TArray<UItemInstanceBase*> ItemsToDrop;

	const int32 SlotBonus = BackpackData ? FMath::Max(0, BackpackData->SlotCount) : 0;
	const float WeightBonus = BackpackData ? FMath::Max(0.f, BackpackData->WeightCapacity) : 0.f;
	const int32 NewSlotCount = BaseSlotCount + SlotBonus;

	MaxCarryWeight = BaseMaxCarryWeight + WeightBonus;

	// 슬롯 확장
	if (NewSlotCount >= Slots.Num())
	{
		MaxSlotCount = NewSlotCount;
		Slots.SetNum(NewSlotCount);

		NotifyInventoryChanged();

		return ItemsToDrop;
	}

	// 축소될 영역의 아이템을 먼저 빼놓음
	TArray<TObjectPtr<UItemInstanceBase>> OverflowItems;

	for (int32 Index = NewSlotCount; Index < Slots.Num(); ++Index)
	{
		if (IsValid(Slots[Index]))
		{
			OverflowItems.Add(Slots[Index]);
		}
	}

	Slots.SetNum(NewSlotCount);
	MaxSlotCount = NewSlotCount;

	// 남아 있는 빈칸으로 이동
	for (UItemInstanceBase* Item : OverflowItems)
	{
		if (!IsValid(Item))
		{
			continue;
		}

		int32 EmptySlotIndex = -1;
		FindEmptySlotIndex(EmptySlotIndex);

		if (Slots.IsValidIndex(EmptySlotIndex))
		{
			Slots[EmptySlotIndex] = Item;
		}
		else
		{
			ItemsToDrop.Add(Item);
		}
	}

	NotifyInventoryChanged();

	return ItemsToDrop;
}

int32 UPlayerInventoryComponent::GetEquipmentSlotIndex(EEquipmentSlot Slot) const
{
	switch (Slot)
	{
	case EEquipmentSlot::Primary:
		return 0;
	case EEquipmentSlot::Secondary:
		return 1;
	case EEquipmentSlot::Melee:
		return 2;
	case EEquipmentSlot::Throwable:
		return 3;
	case EEquipmentSlot::Effect:
		return 4;
	case EEquipmentSlot::Bag:
		return 5;
	case EEquipmentSlot::Shield:
		return 6;
	default:
		return INDEX_NONE;
	}
}

EEquipmentSlot UPlayerInventoryComponent::GetEquipmentSlotType(int32 SlotIndex) const
{
	switch (SlotIndex)
	{
	case 0:
		return EEquipmentSlot::Primary;
	case 1:
		return EEquipmentSlot::Secondary;
	case 2:
		return EEquipmentSlot::Melee;
	case 3:
		return EEquipmentSlot::Throwable;
	case 4:
		return EEquipmentSlot::Effect;
	case 5:
		return EEquipmentSlot::Bag;
	case 6:
		return EEquipmentSlot::Shield;
	default:
		return EEquipmentSlot::Unarmed;
	}
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
		if (!IsValid(NewSlots[i]))
		{
			EquipmentSlots[i] = nullptr;
			continue;
		}

		EquipmentSlots[i] = DuplicateObject<UItemInstanceBase>(NewSlots[i], this);

		if (IsValid(EquipmentSlots[i]))
		{
			EquipmentSlots[i]->Initialize();
		}
	}

	for (int32 i = 0; i < EquipmentSlots.Num(); ++i)
	{
		const EEquipmentSlot Slot = GetEquipmentSlotType(i);

		UItemInstanceBase* PreviousItem = PreviousSlots.IsValidIndex(i) ? PreviousSlots[i].Get() : nullptr;
		UItemInstanceBase* NewItem = EquipmentSlots[i];

		if (PreviousItem != NewItem)
		{
			OnEquipmentItemChanged.Broadcast(Slot, NewItem);
		}

		OnEquipmentSlotChanged.Broadcast(Slot, NewItem);
	}

	RecalculateCarryWeight();

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

	RecalculateCarryWeight();

	return true;
}
