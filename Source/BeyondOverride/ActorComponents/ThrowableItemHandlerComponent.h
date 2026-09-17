#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"

#include "ThrowableItemHandlerComponent.generated.h"

class UThrowableItemInstance;
class UEquippableItemInstance;
class AThrowableProjectile;

struct FThrowableItemDataRow;

// 사용하여 아이템 개수 변경 알림 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCountUpdatedDelegate,
	UEquippableItemInstance*);

UCLASS()
class BEYONDOVERRIDE_API UThrowableItemHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  public:
	// 아이템 사용 후 개수 변경 알림 델리게이트
	FOnCountUpdatedDelegate OnCountUpdatedDelegate;

  protected:
	// 등록된 Throwable Item 인스턴스
	UPROPERTY()
	TObjectPtr<UThrowableItemInstance> ThrowableItemInstance;

	// Throwable Item 데이터
	const FThrowableItemDataRow* ThrowableItemData;

  public:
	UThrowableItemHandlerComponent();

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
	// 투척 타이머
	FTimerHandle ThrowTimerHandle;

  protected:
	// 투척 가능 여부
	bool CanThrow() const;

	// 실제 목표 방향
	FRotator GetAimRotation() const;
	// 투척 시작 위치 반환
	FVector GetThrowStartLocation() const;

	// 투척 시작
	void StartThrow();

	// 투척 수행
	void Throw();

	// 투척 아이템 소환
	AThrowableProjectile* SpawnThrowable();
};
