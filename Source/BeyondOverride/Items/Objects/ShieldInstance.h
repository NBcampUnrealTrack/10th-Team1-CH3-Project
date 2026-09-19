#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "ShieldInstance.generated.h"

struct FShieldDataRow;

UCLASS()
class BEYONDOVERRIDE_API UShieldInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	const FShieldDataRow* ShieldData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	int32 CurrentShield; // 현재 실드량

  public:
	UShieldInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Data
	const FShieldDataRow* GetShieldData() const;

	// Shield
	int32 GetCurrentShield() const;  // 현재 실드량 반환
	void ModifyShield(int32 Delta); // 실드량 변경 - 증가 & 감소
};
