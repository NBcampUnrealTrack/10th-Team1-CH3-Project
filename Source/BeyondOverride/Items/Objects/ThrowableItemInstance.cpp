#include "Items/Objects/ThrowableItemInstance.h"

#include "DataTables/Items/ThrowableItemDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UThrowableItemInstance::UThrowableItemInstance()
{
	ThrowableItemData = nullptr;
}

void UThrowableItemInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[UThrowableItemInstance] Initialize %s"), *GetNameSafe(this));

	// ThrowableItemData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	ThrowableItemData = ItemDataSubsystem->GetThrowableItemData(ItemID);
}

const FThrowableItemDataRow* UThrowableItemInstance::GetThrowableItemData() const
{
	return ThrowableItemData;
}
