#pragma once

#include "CoreMinimal.h"

class UItemInstanceBase;
class AItemPickupBase;

struct BEYONDOVERRIDE_API FItemFactory
{
  public:
	// 아이템 오브젝트 생성 - ItemID
	static UItemInstanceBase* CreateItemInstance(
		UObject* Outer,
		const FName ItemID,
		const int32 StackCount = 0);

	// 아이템 액터 소환 - ItemID
	static AItemPickupBase* SpawnItemPickup(
		UWorld* World,
		const FName ItemID,
		const int32 StackCount = 1,
		const FVector& Location = FVector::ZeroVector,
		const FRotator& Rotation = FRotator::ZeroRotator);

	// 아이템 액터 소환 - ItemInstance
	static AItemPickupBase* SpawnItemPickup(
		UWorld* World,
		UItemInstanceBase* ItemInstance,
		const FVector& Location = FVector::ZeroVector,
		const FRotator& Rotation = FRotator::ZeroRotator);
};
