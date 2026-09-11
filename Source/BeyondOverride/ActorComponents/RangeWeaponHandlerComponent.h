#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"

#include "RangeWeaponHandlerComponent.generated.h"

UCLASS()
class BEYONDOVERRIDE_API URangeWeaponHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  public:
	URangeWeaponHandlerComponent();

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* EquippableItemInstance) override;
	// 장비 등록 해제
	virtual bool Unassign() override;

	// 장비 장착
	virtual bool Equip() override;
	// 장비 해제
	virtual bool Unequip() override;

	// 장비 사용
	virtual bool Use() override;
};
