#include "Items/Objects/ShieldInstance.h"

#include "DataTables/Items/ShieldDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UShieldInstance::UShieldInstance()
{
	ShieldData = nullptr;

	CurrentShield = 0;
}

void UShieldInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[UShieldInstance&] Initialize %s"), *GetNameSafe(this));

	// ShieldData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	ShieldData = ItemDataSubsystem->GetShieldData(ItemID);

	if (!ShieldData)
	{
		CurrentShield = 0;
		return;
	}

	if (!bShieldInitialized)
	{
		CurrentShield = ShieldData->MaxShield;
		bShieldInitialized = true;
	}
	else
	{
		CurrentShield = FMath::Clamp(CurrentShield, 0, ShieldData->MaxShield);
	}
}

const FShieldDataRow* UShieldInstance::GetShieldData() const
{
	return ShieldData;
}

int32 UShieldInstance::GetCurrentShield() const
{
	return CurrentShield;
}

void UShieldInstance::ModifyShield(int32 Delta)
{
	if (!ShieldData)
	{
		return;
	}

	CurrentShield = FMath::Clamp(CurrentShield + Delta, 0, ShieldData->MaxShield);
}
