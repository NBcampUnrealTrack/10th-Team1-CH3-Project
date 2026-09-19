#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "BackpackInstance.generated.h"

struct FBackpackDataRow;

UCLASS()
class BEYONDOVERRIDE_API UBackpackInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	const FBackpackDataRow* BackpackData;

  public:
	UBackpackInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Data
	const FBackpackDataRow* GetBackpackData() const;
};
