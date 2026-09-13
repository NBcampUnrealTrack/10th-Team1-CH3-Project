// TODO:
//	- 비무장 구현
//		- Unarmed(MeleeWeaponInstance) 생성 및 저장
//		- 비무장 시 UnarmedHandler(MeleeWeaponHandlerComponent)에 Unarmed 장비 등록하기
//		- 비무장 전환(Unequip) 또는 빈 슬롯 전환(Equip) 시 UnarmedHandler을 장착하기
//	- 결합도 낮추기
//		- UBOAnimInstance를 직접 불러오지 않고, 델리게이트 등을 통해 전달하기

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentManagerComponent.generated.h"

enum class EEquipmentSlot : uint8;

class UEquipmentHandlerComponent;
class UItemInstanceBase;
class URangeWeaponInstance;

// [RangeWeapon] 재장전 가능한지 확인하는 델리게이트 - 여분 탄약 개수 등 확인하여 재장전 가능 여부 반환
DECLARE_DELEGATE_RetVal_OneParam(
	bool, // 재장전 여부 반환
	FCanReloadDelegate,
	URangeWeaponInstance*);
// [RangeWeapon] 재장전 탄약 요청하는 델리게이트 - 재장전에 사용할 탄약 소모 및 전달
DECLARE_DELEGATE_RetVal_OneParam(
	int32,
	FRequestReloadAmmoDelegate,
	URangeWeaponInstance*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

  protected:
	// 활성화된 장비 슬롯
	EEquipmentSlot ActiveSlot;
	// 각 장비 슬롯 별 컴포넌트
	TMap<EEquipmentSlot, UEquipmentHandlerComponent*> EquipmentHandlerComponents;

  public:
	UEquipmentManagerComponent();

	// Getters
	EEquipmentSlot GetActiveSlot() const;         // 현재 활성화 슬롯 반환
	bool HasEquipment(EEquipmentSlot Slot) const; // 슬롯에 장비가 등록되었는지 여부

  protected:
	virtual void OnRegister() override;

  public:
	// 슬롯 전환
	bool Equip(EEquipmentSlot Slot);
	// 비무장 전환
	bool Unequip(); // 미구현

	// 현재 장비 사용
	bool Use();

	// 현재 장비 재장전 - RangeWeapon 전용
	bool Reload();

	// 슬롯에 장비 등록
	bool Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase);
	// 슬롯에서 장비 해제
	bool Unassign(EEquipmentSlot Slot);

  public:
	// 재장전 가능 여부 델리게이트
	FCanReloadDelegate CanReloadDelegate;
	// 재장전 탄약 요청 델리게이트
	FRequestReloadAmmoDelegate RequestReloadAmmoDelegate;

  protected:
	// 델리게이트 바인딩
	void BindDelegates();

	// Range Weapon 델리게이트
	bool OnCanReload(URangeWeaponInstance* RangeWeaponInstance) const;
	int32 OnRequestReloadAmmo(URangeWeaponInstance* RangeWeaponInstance) const;
};
