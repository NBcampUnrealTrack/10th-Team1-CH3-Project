#include "Factory/ItemFactory.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Subsystems/ItemDataSubsystem.h"

UItemInstanceBase* FItemFactory::CreateItemInstance(
	UObject* Outer,
	const FName ItemID,
	const int32 StackCount)
{
	// Outer & World 유효성 검사
	if (!Outer || !Outer->GetWorld())
	{
		return nullptr;
	}

	// GameInstance 확인
	UGameInstance* GameInstance = Outer->GetWorld()->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	// ItemDataSubsystem 확인
	UItemDataSubsystem* ItemDataSubsystem = GameInstance->GetSubsystem<UItemDataSubsystem>();
	if (!ItemDataSubsystem)
	{
		return nullptr;
	}

	// ItemData 확인
	const FItemDataRow* ItemData = ItemDataSubsystem->GetItemData(ItemID);
	if (!ItemData || !ItemData->ItemInstanceClass)
	{
		return nullptr;
	}

	// ItemInstance 생성 및 확인
	UItemInstanceBase* ItemInstance = NewObject<UItemInstanceBase>(Outer, ItemData->ItemInstanceClass);
	if (!ItemInstance)
	{
		return nullptr;
	}

	// 데이터 초기 설정
	ItemInstance->Initialize();
	ItemInstance->SetStackCount(StackCount);

	return ItemInstance;
}

AItemPickupBase* FItemFactory::SpawnItemPickup(
	UWorld* World,
	const FName ItemID,
	const int32 StackCount,
	const FVector& Location,
	const FRotator& Rotation)
{
	// World 유효성 검사
	if (!World)
	{
		return nullptr;
	}

	// 아이템 오브젝트 생성
	UItemInstanceBase* ItemInstance = CreateItemInstance(
		World,
		ItemID,
		StackCount);
	if (!ItemInstance)
	{
		return nullptr;
	}

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

AItemPickupBase* FItemFactory::SpawnItemPickup(
	UItemInstanceBase* ItemInstance,
	const FVector& Location,
	const FRotator& Rotation)
{
	return nullptr;
}
