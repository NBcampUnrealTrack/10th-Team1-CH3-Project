#include "Items/Objects/ItemInstanceBase.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Subsystems/ItemDataSubsystem.h"

UItemInstanceBase::UItemInstanceBase()
{
	ItemData = nullptr;

	StackCount = 1;
}

void UItemInstanceBase::Initialize()
{
	// ItemData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	ItemData = ItemDataSubsystem->GetItemData(ItemID);
}

const FItemDataRow* UItemInstanceBase::GetItemData() const
{
	return ItemData;
}

FName UItemInstanceBase::GetItemID() const
{
	return ItemID;
}

int32 UItemInstanceBase::GetStackCount() const
{
	return StackCount;
}

void UItemInstanceBase::SetStackCount(int32 InStackCount)
{
	StackCount = FMath::Clamp(InStackCount, 1, ItemData->MaxStackCount);
}
