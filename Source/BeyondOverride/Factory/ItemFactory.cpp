#include "Factory/ItemFactory.h"

#include "Items/Actors/ItemPickupBase.h"
#include "Items/Objects/ItemInstanceBase.h"

UItemInstanceBase* FItemFactory::CreateItemInstance(
	UObject* Outer,
	FName ItemID)
{
	return nullptr;
}

AItemPickupBase* FItemFactory::SpawnItemPickup(
	UWorld* World,
	FName ItemID,
	const FVector& Location,
	const FRotator& Rotation)
{
	return nullptr;
}
