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
	virtual bool Assign(UEquippableItemInstance* InEquippableItemInstance) override;
	// 장비 제거
	virtual UEquippableItemInstance* Unassign() override;

	// 장비 장착
	virtual bool Equip() override;
	// 장비 해제
	virtual bool Unequip() override;

	// 장비 사용
	virtual bool Use() override;

  protected:
	// 장비 등록 가능 여부
	virtual bool CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const override;
	// 장비 제거 가능 여부
	virtual bool CanUnassign() const override;
	// 장비 장착 가능 여부
	virtual bool CanEquip() const override;
	// 장비 해제 가능 여부
	virtual bool CanUnequip() const override;
	// 장비 사용 가능 여부
	virtual bool CanUse() const override;

  protected:
	// 공격 타이머
	FTimerHandle AttackTimerHandle;

  protected:
	// 공격 가능 여부
	bool CanAttack() const;

	// 공격 타이머 시작
	void StartAttackTimer();

	// 공격 애니메이션 재생
	void PlayAttackAnimation();
};
