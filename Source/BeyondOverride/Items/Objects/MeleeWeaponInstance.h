#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "MeleeWeaponInstance.generated.h"

struct FMeleeWeaponDataRow;

UCLASS()
class BEYONDOVERRIDE_API UMeleeWeaponInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	const FMeleeWeaponDataRow* MeleeWeaponData;

  public:
	UMeleeWeaponInstance();

	// 아이템 정보 초기 로드
	virtual void Initialize() override;

	// Data
	const FMeleeWeaponDataRow* GetMeleeWeaponData() const;
};
