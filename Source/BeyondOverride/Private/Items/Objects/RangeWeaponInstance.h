#pragma once

#include "CoreMinimal.h"

#include "Items/Objects/EquippableItemInstance.h"

#include "RangeWeaponInstance.generated.h"

class URangeWeaponDataAsset;

UCLASS()
class URangeWeaponInstance : public UEquippableItemInstance
{
	GENERATED_BODY()

  protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<URangeWeaponDataAsset> RangeWeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	int32 CurrentAmmo;

  public:
	URangeWeaponInstance();

	// Data
	const URangeWeaponDataAsset* GetRangeWeaponData() const;

	// Ammo
	bool ConsumeAmmo();            // 탄약 1개 소모 (소모 성공 여부 반환)
	int32 AddAmmo(int32 Amount);   // 탄약 추가 (추가 후 남은 개수 반환)
	int32 GetCurrentAmmo() const;  // 현재 탄약 개수 반환
};
