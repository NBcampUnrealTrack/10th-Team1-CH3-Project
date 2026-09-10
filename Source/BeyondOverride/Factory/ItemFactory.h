#pragma once

#include "CoreMinimal.h"

class UItemInstanceBase;
class AItemPickupBase;

struct BEYONDOVERRIDE_API FItemFactory
{
  public:
	// 아이템 오브젝트 생성
	static UItemInstanceBase* CreateItemInstance(
		UWorld* World,
		UObject* Outer,
		FName ItemID);

	// 아이템 액터 소환
	static AItemPickupBase* SpawnItemPickup(
		UWorld* World,
		FName ItemID,
		const FVector& Location,
		const FRotator& Rotation);
};
