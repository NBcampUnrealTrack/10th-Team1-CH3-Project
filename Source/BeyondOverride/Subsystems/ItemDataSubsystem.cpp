#include "Subsystems/ItemDataSubsystem.h"

#include "DataAssets/ItemDataRegistry.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/ItemDataRow.h"
#include "DataTables/Items/MeleeWeaponDataRow.h"
#include "DataTables/Items/RangeWeaponDataRow.h"
#include "DataTables/Items/ThrowableItemDataRow.h"
#include "DataTables/Items/UtilityItemDataRow.h"

void UItemDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftObjectPath RegistryPath(TEXT("/Game/DataAssets/DA_ItemDataRegistry.DA_ItemDataRegistry"));
	ItemDataRegistry = Cast<UItemDataRegistry>(RegistryPath.TryLoad());
}

void UItemDataSubsystem::Deinitialize()
{
	ItemDataRegistry = nullptr;

	Super::Deinitialize();
}

const FItemDataRow* UItemDataSubsystem::GetItemData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->ItemTable)
	{
		return nullptr;
	}

	const FItemDataRow* ItemData = ItemDataRegistry->ItemTable->FindRow<FItemDataRow>(ItemID, TEXT("UItemDataSubsystem::GetItemData"));

	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return ItemData;
}

const FEquippableItemDataRow* UItemDataSubsystem::GetEquippableItemData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->EquippableItemTable)
	{
		return nullptr;
	}

	const FEquippableItemDataRow* EquippableItemData = ItemDataRegistry->EquippableItemTable->FindRow<FEquippableItemDataRow>(ItemID, TEXT("UItemDataSubsystem::GetEquippableItemData"));

	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquippableItemData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return EquippableItemData;
}

const FRangeWeaponDataRow* UItemDataSubsystem::GetRangeWeaponData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->RangeWeaponTable)
	{
		return nullptr;
	}

	const FRangeWeaponDataRow* RangeWeaponData = ItemDataRegistry->RangeWeaponTable->FindRow<FRangeWeaponDataRow>(ItemID, TEXT("UItemDataSubsystem::GetRangeWeaponData"));

	if (!RangeWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("RangeWeaponData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return RangeWeaponData;
}

const FMeleeWeaponDataRow* UItemDataSubsystem::GetMeleeWeaponData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->MeleeWeaponTable)
	{
		return nullptr;
	}

	const FMeleeWeaponDataRow* MeleeWeaponData = ItemDataRegistry->MeleeWeaponTable->FindRow<FMeleeWeaponDataRow>(ItemID, TEXT("UItemDataSubsystem::GetMeleeWeaponData"));

	if (!MeleeWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeWeaponData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return MeleeWeaponData;
}

const FThrowableItemDataRow* UItemDataSubsystem::GetThrowableItemData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->ThrowableItemTable)
	{
		return nullptr;
	}

	const FThrowableItemDataRow* ThrowableItemData = ItemDataRegistry->ThrowableItemTable->FindRow<FThrowableItemDataRow>(ItemID, TEXT("UItemDataSubsystem::GetThrowableItemData"));

	if (!ThrowableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowableItemData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return ThrowableItemData;
}

const FUtilityItemDataRow* UItemDataSubsystem::GetUtilityItemData(const FName ItemID) const
{
	if (!ItemDataRegistry || !ItemDataRegistry->UtilityItemTable)
	{
		return nullptr;
	}

	const FUtilityItemDataRow* UtilityItemData = ItemDataRegistry->UtilityItemTable->FindRow<FUtilityItemDataRow>(ItemID, TEXT("UItemDataSubsystem::GetUtilityItemData"));

	if (!UtilityItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UtilityItemData not found: %s"), *ItemID.ToString());
		return nullptr;
	}

	return UtilityItemData;
}
