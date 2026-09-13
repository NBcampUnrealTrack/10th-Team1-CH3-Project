#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"
#include "Components/TimelineComponent.h"

#include "RangeWeaponHandlerComponent.generated.h"

class URangeWeaponInstance;
class ABulletProjectile;

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

  protected:
	// 사격 타이머
	FTimerHandle FireTimerHandle;

	// 반동 적용 속도
	float RecoilApplySpeed;

	// 반동 누적 값
	FVector2D RecoilAccumulator;

	// Pitch 반동 (Up 기준)
	FTimeline RecoilPitchTimeline;
	// Yaw 반동 (Right 기준)
	FTimeline RecoilYawTimeline;
	// 탄 퍼짐 각도 (원뿔 영역 내 균일 분포)
	FTimeline SpreadDegreeTimeline;

  protected:
	// 사격 가능 여부 반환
	bool CanFire() const;

	// 타임라인 설정
	void SetupTimeline();
	// 타임라인 제거
	void ClearTimeline();

	// 총구 위치 반환
	FVector GetMuzzleLocation() const;
	// 총구 방향 반환
	FRotator GetMuzzleRotation() const;

	// 현재 누적 반동에 추가
	void AddRecoil();
	// 현재 탄퍼짐 바탕으로 사격 방향 반환
	FRotator GetSpreadRotation(const FRotator& AimRotation);

	// 총알 소환
	ABulletProjectile* SpawnProjectile(
		AActor* Instigator,
		const FVector& StartLocation,
		const FRotator& Rotation);

	// 사격 타이머 활성화
	void StartFireTimer();

	// 사격 애니메이션 재생
	void PlayFireAnimation();
};
