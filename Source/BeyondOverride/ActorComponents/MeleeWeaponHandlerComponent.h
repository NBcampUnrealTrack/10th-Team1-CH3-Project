#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"

#include "MeleeWeaponHandlerComponent.generated.h"

class UMeleeWeaponInstance;

UCLASS()
class BEYONDOVERRIDE_API UMeleeWeaponHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  protected:
	UPROPERTY()
	TObjectPtr<UMeleeWeaponInstance> MeleeWeaponInstance;

  public:
	UMeleeWeaponHandlerComponent();

	// 등록된 장비 반환
	virtual UEquippableItemInstance* GetEquippableItemInstance() const;

	// 장비 등록
	virtual bool Assign(UEquippableItemInstance* EquippableItemInstance) override;
	// 장비 제거
	virtual UEquippableItemInstance* Unassign() override;

	// 장비 장착
	virtual bool Equip() override;
	// 장비 해제
	virtual bool Unequip() override;

	// 장비 사용
	virtual bool Use() override;

	// 장비 해제 가능 여부
	virtual bool CanUnequip() override;
};
