#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"
#include "Components/TimelineComponent.h"

#include "RangeWeaponHandlerComponent.generated.h"

class URangeWeaponInstance;

struct FRangeWeaponDataRow;

// 사격 실행 시 송출하는 델리게이트 - 캐릭터 사격 애니메이션 등 수행
DECLARE_MULTICAST_DELEGATE(
	FOnFireExecutedDelegate);

// 재장전 가능한지 확인하는 델리게이트 - 재장전 가능 검증에 실행하여, 무기의 탄약 ID를 인자로 주고 해당 탄약의 여분이 있는지 여부를 반환함
DECLARE_DELEGATE_RetVal_OneParam(
	bool, // 재장전 가능 여부 반환
	FCanReloadDelegate,
	const FName&); // 탄약 ItemID
// 재장전 탄약 요청하는 델리게이트 - 재장전 종료 시 실행하여, 재장전에 사용할 탄약 ID 및 필요한 개수를 주고 보충 가능한 개수를 반환함
DECLARE_DELEGATE_RetVal_TwoParams(
	int32, // 재장전에 사용할 탄약 개수 반환
	FRequestReloadAmmoDelegate,
	const FName&, // 탄약 ItemID
	const int32); // 필요한 탄약 개수

UCLASS()
class BEYONDOVERRIDE_API URangeWeaponHandlerComponent : public UEquipmentHandlerComponent
{
	GENERATED_BODY()

  public:
	// 사격 수행 시 송출하는 델리게이트
	FOnFireExecutedDelegate OnFireExecutedDelegate;
	// 재장전 가능 여부 델리게이트
	FCanReloadDelegate CanReloadDelegate;
	// 재장전 탄약 요청 델리게이트
	FRequestReloadAmmoDelegate RequestReloadAmmoDelegate;

  protected:
	// 등록된 Range Weapon 인스턴스
	UPROPERTY()
	TObjectPtr<URangeWeaponInstance> RangeWeaponInstance;

	// Range Weapon 데이터
	const FRangeWeaponDataRow* RangeWeaponData;

	// 총구 소켓 이름
	FName MuzzleSocketName;

  public:
	URangeWeaponHandlerComponent();

  protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  public:
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

	// 재장전
	bool Reload();

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
	// 사격 활성화 여부
	bool bIsActive;
	// 조준 여부
	bool bIsAiming;

	// 사격 타이머
	FTimerHandle FireTimerHandle;
	// 재장전 타이머
	FTimerHandle ReloadTimerHandle;

	// 반동 누적 값
	FVector2D RecoilAccumulator;

	// Pitch 반동 (Up 기준)
	FTimeline RecoilPitchTimeline;
	// Yaw 반동 (Right 기준)
	FTimeline RecoilYawTimeline;
	// 탄 퍼짐 각도 (원뿔 영역 내 균일 분포)
	FTimeline SpreadDegreeTimeline;

  protected:
	// 사격
	void Fire();

	// 사격 가능 여부 반환
	bool CanFire() const;
	// 재장전 가능 여부 반환
	bool CanReload() const;

	// 타임라인 설정
	void SetupTimeline();
	// 타임라인 제거
	void ClearTimeline();
	// 타임라인 재생
	void PlayTimeline(bool bReverse = false);

	// 총구 위치 반환
	FVector GetMuzzleLocation() const;
	// 총구 방향 반환
	FRotator GetMuzzleRotation() const;
	// 실제 목표 방향
	FRotator GetAimRotation() const;

	// 현재 누적 반동에 추가
	void AddRecoil();
	// 현재 탄퍼짐 바탕으로 사격 방향 반환
	FRotator GetSpreadRotation(const FRotator& AimRotation);

	// 총알 소환
	void SpawnBullets();

	// 사격 타이머 활성화
	void StartFireTimer();
	// 재장전 타이머 활성화
	void StartReloadTimer();

	// 사격 애니메이션 재생
	void PlayFireAnimation();
	// 재장전 애니메이션 재생
	void PlayReloadAnimation();
	// 재장전 애니메이션 중단
	void StopReloadAnimation();

  protected:
	// 사격 종료 시 호출
	void OnFireCompleted();

	// 재장전 시작 시 호출
	void OnReloadStarted();
	// 재장전 종료 시 호출
	void OnReloadCompleted();
	// 재장전 중단 시 호출
	void OnReloadInterrupted();
};
