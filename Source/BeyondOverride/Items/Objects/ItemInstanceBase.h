#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "ItemInstanceBase.generated.h"

class AItemPickupBase;

struct FItemDataRow;

UCLASS(BlueprintType, Blueprintable)
class UItemInstanceBase : public UObject
{
	GENERATED_BODY()

  protected:
	TObjectPtr<const FItemDataRow> ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
	FName ItemID;  // 아이템 ID

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int32 StackCount;

  public:
	UItemInstanceBase();

	// 아이템 정보 초기 로드
	virtual void Initialize();

	// 아이템 액터 소환
	AItemPickupBase* SpawnPickup(
		const FVector& Location = FVector::ZeroVector,
		const FRotator& Rotation = FRotator::ZeroRotator);

	// Getters
	const FItemDataRow* GetItemData() const;
	int32 GetStackCount() const;
};
