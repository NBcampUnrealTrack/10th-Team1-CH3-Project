#pragma once

#include "CoreMinimal.h"

#include "ActorComponents/EquipmentHandlerComponent.h"
#include "Components/TimelineComponent.h"

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

  protected:
	// 반동 적용 속도
	float RecoilApplySpeed;
	// 반동 누적 값
	FVector2D RecoilAccumulator;

	// Pitch 반동 (Up 기준)
	FTimeline RecoilPitchTimeline;
	FOnTimelineFloat RecoilPitchTimelineCallback;
	// Yaw 반동 (Right 기준)
	FTimeline RecoilYawTimeline;
	FOnTimelineFloat RecoilYawTimelineCallback;
	// 탄 퍼짐 각도 (원뿔 영역 내 균일 분포)
	FTimeline SpreadDegreeTimeline;
	FOnTimelineFloat SpreadDegreeTimelineCallback;

  protected:
	void AddRecoil();                                        // 현재 누적 반동에 추가
	FRotator GetSpreadRotation(const FRotator& AimRotation); // 현재 탄퍼짐 바탕으로 사격 방향 반환
};
