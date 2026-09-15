#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentManagerComponent.generated.h"

enum class EEquipmentSlot : uint8;

class UEquipmentHandlerComponent;
class UItemInstanceBase;
class UEquippableItemInstance;
class URangeWeaponInstance;

// [UEquipmentManagerComponent] 활성화 슬롯 전환 시 송출하는 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnActiveSlotChangedDelegate,
	EEquipmentSlot,
	UEquippableItemInstance*);

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

// [Throwable & Effect Item] 아이템 사용 시 개수 변경 알리는 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnEquipmentStackCountUpdatedDelegate,
	EEquipmentSlot,
	UEquippableItemInstance*);

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

	// 초기화 - Owner의 BeginPlay() 시 호출
	void Initialize();

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
	UItemInstanceBase* Unassign(EEquipmentSlot Slot);

  public:
	// 장비 인스턴스 전달 델리게이트
	FOnActiveSlotChangedDelegate OnActiveSlotChangedDelegate;

	// 재장전 가능 여부 델리게이트
	FCanReloadDelegate CanReloadDelegate;
	// 재장전 탄약 요청 델리게이트
	FRequestReloadAmmoDelegate RequestReloadAmmoDelegate;

	// 사용 후 아이템 개수 변경 알림 델리게이트
	FOnEquipmentStackCountUpdatedDelegate OnEquipmentStackCountUpdatedDelegate;

  protected:
	// 델리게이트 바인딩
	void BindDelegates();

	// [TRange Weapon] 델리게이트 바인딩 이벤트
	bool OnCanReload(URangeWeaponInstance* RangeWeaponInstance) const;
	int32 OnRequestReloadAmmo(URangeWeaponInstance* RangeWeaponInstance) const;

	// [Throwable & Effect Item] 델리게이트 바인딩 이벤트
	void OnEquipmentCountUpdated(UEquippableItemInstance* EquippableItemInstance);
};
