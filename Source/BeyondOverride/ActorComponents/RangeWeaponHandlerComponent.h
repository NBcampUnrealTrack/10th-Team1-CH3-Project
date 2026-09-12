#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"

#include "RangeWeaponHandlerComponent.generated.h"

class URangeWeaponInstance;

UCLASS()
class BEYONDOVERRIDE_API URangeWeaponHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  protected:
	TObjectPtr<URangeWeaponInstance> RangeWeaponInstance;

  public:
	URangeWeaponHandlerComponent();

	// 등록된 장비 반환
	virtual UEquippableItemInstance* GetEquippableItemInstance() const;

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* EquippableItemInstance) override;
	// 장비 제거
	virtual bool Unassign() override;

	// 장비 장착
	virtual bool Equip() override;
	// 장비 해제
	virtual bool Unequip() override;

	// 장비 사용
	virtual bool Use() override;

  protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
