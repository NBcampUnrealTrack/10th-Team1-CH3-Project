#include "Items/Objects/MeleeWeaponInstance.h"

#include "DataTables/Items/MeleeWeaponDataRow.h"
#include "Subsystems/ItemDataSubsystem.h"

UMeleeWeaponInstance::UMeleeWeaponInstance()
{
	MeleeWeaponData = nullptr;
}

void UMeleeWeaponInstance::Initialize()
{
	Super::Initialize();

	UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponInstance] Initialize %s"), *GetNameSafe(this));

	// MeleeWeaponData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	MeleeWeaponData = ItemDataSubsystem->GetMeleeWeaponData(ItemID);
}

const FMeleeWeaponDataRow* UMeleeWeaponInstance::GetMeleeWeaponData() const
{
	return MeleeWeaponData;
}
