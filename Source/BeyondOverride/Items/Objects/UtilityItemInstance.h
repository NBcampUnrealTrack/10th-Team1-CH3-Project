#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "UtilityItemInstance.generated.h"

struct FUtilityItemDataRow;

UCLASS()
class BEYONDOVERRIDE_API UUtilityItemInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	const FUtilityItemDataRow* UtilityItemData;

  public:
	UUtilityItemInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Data
	const FUtilityItemDataRow* GetUtilityItemData() const;
};
