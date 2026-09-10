#include "Factory/ItemFactory.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Subsystems/ItemDataSubsystem.h"

UItemInstanceBase* FItemFactory::CreateItemInstance(
	UWorld* World,
	UObject* Outer,
	FName ItemID)
{
	if (!World)
	{
		return nullptr;
	}

	// TODO

	return nullptr;
}

AItemPickupBase* FItemFactory::SpawnItemPickup(
	UWorld* World,
	FName ItemID,
	const FVector& Location,
	const FRotator& Rotation)
{
	if (!World)
	{
		return nullptr;
	}

	// 아이템 오브젝트 생성
	UItemInstanceBase* ItemInstance = CreateItemInstance(
		World,
		nullptr,
		ItemID);

	// 아이템 데이터 확인
	const FItemDataRow* ItemData = ItemInstance->GetItemData();
	if (!ItemData)
	{
		return nullptr;
	}

	// 액터 클래스 확인
	TSubclassOf<AItemPickupBase> ItemPickupClass = ItemData->ItemPickupClass;
	if (!ItemPickupClass)
	{
		return nullptr;
	}

	// 액터 생성
	AItemPickupBase* ItemPickup = World->SpawnActor<AItemPickupBase>(
		ItemPickupClass,
		Location,
		Rotation);
	if (!ItemPickup)
	{
		return nullptr;
	}

	// 액터에 오브젝트 저장
	ItemPickup->Initialize(ItemInstance);

	return ItemPickup;
}
