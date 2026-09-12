#include "ActorComponents/RangeWeaponHandlerComponent.h"

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

void URangeWeaponHandlerComponent::AddRecoil()
{
	// TODO: 반동 타임라인을 통해 누적 반동에 추가
}

FRotator URangeWeaponHandlerComponent::GetSpreadRotation(const FRotator& AimRotation)
{
	// TODO: 탄 퍼짐 타임라인을 통해 균일 분포를 적용한 방향 반환

	return AimRotation;
}
