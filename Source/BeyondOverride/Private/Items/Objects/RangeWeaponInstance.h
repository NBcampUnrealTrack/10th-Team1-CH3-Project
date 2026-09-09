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
	bool ConsumeAmmo();
	void AddAmmo(int32& Amount);
	int32 GetCurrentAmmo() const;
};
