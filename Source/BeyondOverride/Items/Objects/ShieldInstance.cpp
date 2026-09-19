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
	CurrentShield += Delta;

	if (ShieldData)
	{
		CurrentShield = FMath::Clamp(CurrentShield, 0, ShieldData->MaxShield); // 음수 & 실드 최대치 초과 방지
	}
	else
	{
		CurrentShield = FMath::Max(0, CurrentShield); // 음수 방지
	}
}
