#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "ThrowableItemInstance.generated.h"

struct FThrowableItemDataRow;

UCLASS()
class BEYONDOVERRIDE_API UThrowableItemInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	const FThrowableItemDataRow* ThrowableItemData;

  public:
	UThrowableItemInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Data
	const FThrowableItemDataRow* GetThrowableItemData() const;
};
