#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/RangeWeaponDataRow.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Kismet/KismetMathLibrary.h"

URangeWeaponHandlerComponent::URangeWeaponHandlerComponent()
{
	// 틱 가능 & 초기 비활성화
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// 총구 소켓 이름
	MuzzleSocketName = FName("Muzzle");

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

	// 장비 메시 설정
	if (EquipMeshComponent)
	{
		if (USkeletalMesh* Mesh = RangeWeaponInstance->GetEquippableItemData()->EquipMesh)
		{
			EquipMeshComponent->SetSkeletalMesh(Mesh);
		}
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

	// 장비 메시 제거
	if (EquipMeshComponent)
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

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
	// 사격 불가
	if (!CanFire())
	{
		return false;
	}

	// 총구 위치 & 방향
	const FVector MuzzleLocation = GetMuzzleLocation();
	const FRotator AimRotation = GetAimRotation();

	// 총알 소환
	ABulletProjectile* Bullet = SpawnProjectile(
		GetOwner(),
		MuzzleLocation,
		GetSpreadRotation(AimRotation)); // 탄 퍼짐 적용

	// 반동 추가
	AddRecoil();

	// 사격 애니메이션 재생
	PlayFireAnimation();

	// 탄약 소모
	RangeWeaponInstance->ConsumeAmmo();

	// 사격 쿨다운 설정
	StartFireTimer();

	return true;
}

bool URangeWeaponHandlerComponent::Reload()
{
	// 재장전 불가
	if (!CanReload())
	{
		return false;
	}

	OnReloadStarted();

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

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
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

bool URangeWeaponHandlerComponent::CanReload() const
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return false;
	}

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return false;
	}

	// 재장전 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
	{
		return false;
	}

	// 여분 탄약 등, 외부 조건 확인
	if (!CanReloadDeleagte.IsBound() || !CanReloadDeleagte.Execute(RangeWeaponInstance))
	{
		return false;
	}

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

FVector URangeWeaponHandlerComponent::GetMuzzleLocation() const
{
	FVector MuzzleLocation = GetOwner()->GetActorLocation();
	if (EquipMeshComponent && EquipMeshComponent->DoesSocketExist(MuzzleSocketName))
	{
		MuzzleLocation = EquipMeshComponent->GetSocketLocation(MuzzleSocketName);
	}

	return MuzzleLocation;
}

FRotator URangeWeaponHandlerComponent::GetMuzzleRotation() const
{
	FRotator Muzzleotation = GetOwner()->GetActorRotation();
	if (EquipMeshComponent && EquipMeshComponent->DoesSocketExist(MuzzleSocketName))
	{
		Muzzleotation = EquipMeshComponent->GetSocketRotation(MuzzleSocketName);
	}

	return Muzzleotation;
}

FRotator URangeWeaponHandlerComponent::GetAimRotation() const
{
	// 총구 방향
	const FRotator MuzzleRotation = GetMuzzleRotation();

	// Owner 유효성 검증
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return MuzzleRotation;
	}

	// 컨트롤러 유효성 검증
	AController* Controller = Pawn->GetController();
	if (!Controller)
	{
		return MuzzleRotation;
	}

	// 컨트롤러 위치 & 방향
	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	// 라인트레이스 실행 - 컨트롤러 기준
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	const FVector StartLocation = ViewLocation;
	const FVector EndLocation = StartLocation + ViewRotation.Vector() * 1e6f; // 10km

	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		Params);

	// 목표 위치
	const FVector AimLocation = HitResult.bBlockingHit
									? HitResult.ImpactPoint
									: EndLocation;

	// 총구 방향 구하기
	const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(
		GetMuzzleLocation(),
		AimLocation);

	return AimRotation;
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
	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return AimRotation;
	}

	// SpreadCurve 유효성 검증
	const UCurveFloat* SpreadCurve = RangeWeaponData->SpreadCurve;
	if (!SpreadCurve)
	{
		return AimRotation;
	}

	// 탄 퍼짐 각도
	const float SpreadDegree = SpreadCurve->GetFloatValue(SpreadDegreeTimeline.GetPlaybackPosition());
	const float SpreadRadians = FMath::DegreesToRadians(SpreadDegree);

	// 원뿔 내 균일 분포
	return FMath::VRandCone(
			   AimRotation.Vector(),
			   SpreadRadians)
		.Rotation();
}

ABulletProjectile* URangeWeaponHandlerComponent::SpawnProjectile(
	AActor* Instigator,
	const FVector& StartLocation,
	const FRotator& Rotation)
{
	return nullptr;
}

void URangeWeaponHandlerComponent::StartFireTimer()
{
	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (RangeWeaponData)
	{
		return;
	}

	// 사격 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,
		RangeWeaponData->FireRate,
		false);
}

void URangeWeaponHandlerComponent::StartReloadTimer()
{
	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (RangeWeaponData)
	{
		return;
	}

	// 재장전 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&URangeWeaponHandlerComponent::OnReloadCompleted,
		RangeWeaponData->ReloadTime,
		false);
}

void URangeWeaponHandlerComponent::PlayFireAnimation()
{
}

void URangeWeaponHandlerComponent::ReloadFireAnimation()
{
}

void URangeWeaponHandlerComponent::OnReloadStarted()
{
	// 재장전 애니메이션 재생
	ReloadFireAnimation();

	// 재장전 타이머 활성화
	StartReloadTimer();
}

void URangeWeaponHandlerComponent::OnReloadCompleted()
{
	// 추가할 탄약 개수
	const int32 AddedAmmo = CanReloadDeleagte.IsBound()
								? RequestReloadAmmoDelegate.Execute(RangeWeaponInstance)
								: 0;
	// 탄약 추가
	RangeWeaponInstance->AddAmmo(AddedAmmo);
}
