#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"

#include "UtilityItemHandlerComponent.generated.h"

class UEquippableItemInstance;
class UUtilityItemInstance;

struct FUtilityItemDataRow;

// 사용하여 아이템 개수 변경 알림 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCountUpdatedDelegate,
	UEquippableItemInstance*);

// 효과 적용 알림 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnEffectAppliedDelegate,
	UUtilityItemInstance*);

UCLASS()
class BEYONDOVERRIDE_API UUtilityItemHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  public:
	// 아이템 사용 후 개수 변경 알림 델리게이트
	FOnCountUpdatedDelegate OnCountUpdatedDelegate;
	// 아이템 사용 후 효과 적용 알림 델리게이트
	FOnEffectAppliedDelegate OnEffectAppliedDelegate;

  protected:
	// 등록된 Utility Item 인스턴스
	UPROPERTY()
	TObjectPtr<UUtilityItemInstance> UtilityItemInstance;

	// Utility Item 데이터
	const FUtilityItemDataRow* UtilityItemData;

  public:
	UUtilityItemHandlerComponent();

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

	// 사용 시작
	virtual void StartAction();
	// 사용 종료
	virtual void EndAction();

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
	// 사용 타이머
	FTimerHandle UseTimerHandle;

	// 사용 시작
	void OnUseStarted();
	// 사용 완료
	void OnUseCompleted();
	// 사용 중단
	void OnUseInterrupted();
};
