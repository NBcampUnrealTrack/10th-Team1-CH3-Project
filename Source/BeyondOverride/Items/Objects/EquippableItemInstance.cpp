#include "Items/Objects/EquippableItemInstance.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UEquippableItemInstance::UEquippableItemInstance()
{
	EquippableItemData = nullptr;
}

void UEquippableItemInstance::Initialize()
{
	Super::Initialize();

	// EquippableItemData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	EquippableItemData = ItemDataSubsystem->GetEquippableItemData(ItemID);
}

const FEquippableItemDataRow* UEquippableItemInstance::GetEquippableItemData() const
{
	return EquippableItemData;
}
