#include "Items/Objects/BackpackInstance.h"

#include "DataTables/Items/BackpackDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UBackpackInstance::UBackpackInstance()
{
	BackpackData = nullptr;
}

void UBackpackInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[UBackpackInstance&] Initialize %s"), *GetNameSafe(this));

	// BackpackData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	BackpackData = ItemDataSubsystem->GetBackpackData(ItemID);
}

const FBackpackDataRow* UBackpackInstance::GetBackpackData() const
{
	return BackpackData;
}
