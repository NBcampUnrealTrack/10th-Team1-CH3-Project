#include "Items/Objects/ItemInstanceBase.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Subsystems/ItemDataSubsystem.h"

UItemInstanceBase::UItemInstanceBase()
{
	ItemData = nullptr;

	StackCount = 1;
}

void UItemInstanceBase::Initialize()
{
	// ItemData 로드
	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	ItemData = ItemDataSubsystem->GetItemData(ItemID);
}

AItemPickupBase* UItemInstanceBase::SpawnPickup(
	const FVector& Location,
	const FRotator& Rotation)
{
	// 액터 클래스 확인
	TSubclassOf<AItemPickupBase> ItemPickupClass = ItemData->ItemPickupClass;
	if (!ItemPickupClass)
	{
		return nullptr;
	}

	// 액터 생성
	AItemPickupBase* ItemPickup = GetWorld()->SpawnActor<AItemPickupBase>(
		ItemPickupClass,
		Location,
		Rotation);
	if (!ItemPickup)
	{
		return nullptr;
	}

	// 액터에 현재 인스턴스 저장
	ItemPickup->Initialize(this);

	return ItemPickup;
}

const FItemDataRow* UItemInstanceBase::GetItemData() const
{
	return ItemData;
}

int32 UItemInstanceBase::GetStackCount() const
{
	return StackCount;
}
