#include "Items/Objects/UtilityItemInstance.h"

#include "DataTables/Items/UtilityItemDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UUtilityItemInstance::UUtilityItemInstance()
{
	UtilityItemData = nullptr;
}

void UUtilityItemInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[UUtilityItemInstance] Initialize %s"), *GetNameSafe(this));

	// UtilityItemData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	UtilityItemData = ItemDataSubsystem->GetUtilityItemData(ItemID);
}

const FUtilityItemDataRow* UUtilityItemInstance::GetUtilityItemData() const
{
	return UtilityItemData;
}
