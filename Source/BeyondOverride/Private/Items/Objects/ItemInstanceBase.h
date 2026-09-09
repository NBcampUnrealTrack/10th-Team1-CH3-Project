#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "ItemInstanceBase.generated.h"

class UItemDataAsset;
class AItemPickupBase;

UCLASS(BlueprintType, Blueprintable)
class UItemInstanceBase : public UObject
{
	GENERATED_BODY()

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<const UItemDataAsset> ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int32 StackCount;

  public:
	UItemInstanceBase();

	// 아이템 액터 소환
	AItemPickupBase* SpawnPickup(
		const FVector& Location = FVector::ZeroVector,
		const FRotator& Rotation = FRotator::ZeroRotator);

	// Getters
	const UItemDataAsset* GetItemData() const;
	int32 GetStackCount() const;
};
