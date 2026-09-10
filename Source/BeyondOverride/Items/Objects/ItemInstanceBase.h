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
	const FItemDataRow* ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ID")
	FName ItemID;  // 아이템 ID

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	int32 StackCount;

public:
	UItemInstanceBase();

	// 아이템 정보 초기 로드
	virtual void Initialize();

	// Getters
	const FItemDataRow* GetItemData() const;
	int32 GetStackCount() const;

	// Setters
	void SetStackCount(int32 NewStackCount) { StackCount = NewStackCount; }
};
