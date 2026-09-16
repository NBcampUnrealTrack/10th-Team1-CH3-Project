#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "EquipmentManagerComponent.generated.h"

enum class EEquipmentSlot : uint8;

class UEquipmentHandlerComponent;
class UItemInstanceBase;
class UEquippableItemInstance;
class URangeWeaponInstance;
class UUtilityItemInstance;

// [UEquipmentManagerComponent] 활성화 슬롯 전환 시 송출하는 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnActiveSlotChangedDelegate,
	EEquipmentSlot,
	UEquippableItemInstance*);

// [RangeWeapon] 사격 실행 시 송출하는 델리게이트 - 캐릭터 사격 애니메이션 재생 등 수행
DECLARE_MULTICAST_DELEGATE(
	FOnFireExecutedDelegate);

// [RangeWeapon] 재장전 가능한지 확인하는 델리게이트 - 여분 탄약 개수 등 확인하여 재장전 가능 여부 반환
DECLARE_DELEGATE_RetVal_OneParam(
	bool, // 재장전 가능 여부 반환
	FCanReloadDelegate,
	const FName&); // 탄약 ItemID
// [RangeWeapon] 재장전 탄약 요청하는 델리게이트 - 재장전에 사용할 탄약 소모 및 전달
DECLARE_DELEGATE_RetVal_TwoParams(
	int32, // 재장전에 사용할 탄약 개수 반환
	FRequestReloadAmmoDelegate,
	const FName&, // 탄약 ItemID
	const int32); // 필요한 탄약 개수

// [Throwable & Utility Item] 아이템 사용 시 개수 변경 알리는 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnEquipmentStackCountUpdatedDelegate,
	EEquipmentSlot,
	UEquippableItemInstance*);

// [Utility Item] 아이템 사용 완료 후 적용할 효과 알리는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnEffectAppliedDelegate,
	UUtilityItemInstance*);

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

	// 사용 시작
	virtual void StartAction();
	// 사용 종료
	virtual void EndAction();

	// 현재 장비 재장전 - RangeWeapon 전용
	bool Reload();

	// 슬롯에 장비 등록
	bool Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase);
	// 슬롯에서 장비 해제
	UItemInstanceBase* Unassign(EEquipmentSlot Slot);

  public:
	// 장비 인스턴스 전달 델리게이트
	FOnActiveSlotChangedDelegate OnActiveSlotChangedDelegate;

	// 사격 실행 델리게이트
	FOnFireExecutedDelegate OnFireExecutedDelegate;

	// 재장전 가능 여부 델리게이트
	FCanReloadDelegate CanReloadDelegate;
	// 재장전 탄약 요청 델리게이트
	FRequestReloadAmmoDelegate RequestReloadAmmoDelegate;

	// 사용 후 아이템 개수 변경 알림 델리게이트
	FOnEquipmentStackCountUpdatedDelegate OnEquipmentStackCountUpdatedDelegate;

	// 아이템 효과 적용 델리게이트
	FOnEffectAppliedDelegate OnEffectAppliedDelegate;

  protected:
	// 델리게이트 바인딩
	void BindDelegates();

	// [Range Weapon] 델리게이트 바인딩 이벤트
	void OnFireExecuted() const;
	bool CanReload(const FName& AmmoItemID) const;
	int32 RequestReloadAmmo(const FName& AmmoItemID, const int32 RequestedAmmoCount);

	// [Throwable & Utility Item] 델리게이트 바인딩 이벤트
	void OnEquipmentCountUpdated(UEquippableItemInstance* EquippableItemInstance);

	// [Utility Item] 델리게이트 바인딩
	void OnEffectApplied(UUtilityItemInstance* UtilityItemInstance) const;
};
