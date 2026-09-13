#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/ItemInstanceBase.h"

#include "EquippableItemInstance.generated.h"

struct FEquippableItemDataRow;

UCLASS()
class UEquippableItemInstance : public UItemInstanceBase
{
	GENERATED_BODY()

  protected:
	const FEquippableItemDataRow* EquippableItemData;

  public:
	UEquippableItemInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Getters
	const FEquippableItemDataRow* GetEquippableItemData() const;
};
