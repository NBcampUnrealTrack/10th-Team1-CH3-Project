#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "DataTables/Items/RangeWeaponDataRow.h"
#include "GameFramework/Pawn.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"

URangeWeaponHandlerComponent::URangeWeaponHandlerComponent()
{
	// 틱 가능 & 초기 비활성화
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// 반동 적용 속도
	RecoilApplySpeed = 10;
}

UEquippableItemInstance* URangeWeaponHandlerComponent::GetEquippableItemInstance() const
{
	return RangeWeaponInstance;
}

bool URangeWeaponHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	// 이미 등록된 장비 존재
	if (RangeWeaponInstance)
	{
		return false;
	}

	// 잘못된 아이템 장착 시도
	RangeWeaponInstance = Cast<URangeWeaponInstance>(EquippableItemInstance);
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 틱 활성화
	SetComponentTickEnabled(true);

	// 타임라인 설정
	SetupTimeline();

	// 등록 성공
	return true;
}

bool URangeWeaponHandlerComponent::Unassign()
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 장착 해제 시도
	if (!Unequip())
	{
		return false;
	}

	// 장비 제거
	RangeWeaponInstance = nullptr;

	// 틱 비활성화
	SetComponentTickEnabled(false);

	// 타임라인 제거
	ClearTimeline();

	// 제거 성공
	return true;
}

bool URangeWeaponHandlerComponent::Equip()
{
	return true;
}

bool URangeWeaponHandlerComponent::Unequip()
{
	return true;
}

bool URangeWeaponHandlerComponent::Use()
{
	return true;
}

void URangeWeaponHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 적용할 반동 값 계산
	FVector2D RecoilDelta = FMath::Vector2DInterpConstantTo(FVector2D::ZeroVector, RecoilAccumulator, DeltaTime, RecoilApplySpeed);

	// 반동 적용
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	Pawn->AddControllerPitchInput(-RecoilDelta.Y);
	Pawn->AddControllerYawInput(RecoilDelta.X);

	// 누적에 반영
	RecoilAccumulator -= RecoilDelta;
}

bool URangeWeaponHandlerComponent::CanFire() const
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 사격 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		return false;
	}

	// 탄약 부족
	if (RangeWeaponInstance->GetCurrentAmmo() <= 0)
	{
		return false;
	}

	// 사격 가능
	return true;
}

void URangeWeaponHandlerComponent::SetupTimeline()
{
	// 기본 타임라인 제거
	ClearTimeline();

	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return;
	}

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return;
	}

	// 반동 Pitch
	if (UCurveFloat* RecoilPitchCurve = RangeWeaponData->RecoilPitchCurve)
	{
		RecoilPitchTimeline.AddInterpFloat(RecoilPitchCurve, FOnTimelineFloat());
	}

	// 반동 Yaw
	if (UCurveFloat* RecoilYawCurve = RangeWeaponData->RecoilYawCurve)
	{
		RecoilYawTimeline.AddInterpFloat(RecoilYawCurve, FOnTimelineFloat());
	}

	// 탄 퍼짐
	if (UCurveFloat* SpreadCurve = RangeWeaponData->SpreadCurve)
	{
		SpreadDegreeTimeline.AddInterpFloat(SpreadCurve, FOnTimelineFloat());
	}
}

void URangeWeaponHandlerComponent::ClearTimeline()
{
	// 반동 Pitch
	RecoilPitchTimeline = FTimeline();

	// 반동 Yaw
	RecoilYawTimeline = FTimeline();

	// 탄 퍼짐
	SpreadDegreeTimeline = FTimeline();
}

void URangeWeaponHandlerComponent::AddRecoil()
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return;
	}

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return;
	}

	// 반동 Pitch
	if (UCurveFloat* RecoilPitchCurve = RangeWeaponData->RecoilPitchCurve)
	{
		RecoilAccumulator.Y += RecoilPitchCurve->GetFloatValue(RecoilPitchTimeline.GetPlaybackPosition());
	}

	// 반동 Yaw
	if (UCurveFloat* RecoilYawCurve = RangeWeaponData->RecoilYawCurve)
	{
		RecoilAccumulator.X += RecoilYawCurve->GetFloatValue(RecoilYawTimeline.GetPlaybackPosition());
	}
}

FRotator URangeWeaponHandlerComponent::GetSpreadRotation(const FRotator& AimRotation)
{
	// TODO: 탄 퍼짐 타임라인을 통해 균일 분포를 적용한 방향 반환

	return AimRotation;
}
