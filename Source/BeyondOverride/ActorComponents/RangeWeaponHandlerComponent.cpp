#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "DataAssets/EquipmentAnimationDataAsset.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/RangeWeaponDataRow.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/RangeWeaponInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectiles/Bullets/BulletProjectile.h"

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

bool URangeWeaponHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Range Weapon 인스턴스 저장
	RangeWeaponInstance = Cast<URangeWeaponInstance>(EquippableItemInstance);

	// 틱 활성화
	SetComponentTickEnabled(true);

	// 타임라인 설정
	SetupTimeline();

	// 등록 성공
	return true;
}

UEquippableItemInstance* URangeWeaponHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// 틱 비활성화
	SetComponentTickEnabled(false);

	// 타임라인 제거
	ClearTimeline();

	// Range Weapon 인스턴스 제거
	RangeWeaponInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool URangeWeaponHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	return true;
}

bool URangeWeaponHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	// 재장전 중이면 취소
	OnReloadInterrupted();

	return true;
}

bool URangeWeaponHandlerComponent::Use()
{
	// 사격 불가
	if (!CanFire())
	{
		return false;
	}

	// 재장전 중이면, 취소 후 사격
	OnReloadInterrupted();

	// 총구 위치 & 방향
	const FVector MuzzleLocation = GetMuzzleLocation();
	const FRotator AimRotation = GetAimRotation();

	// 총알 소환
	ABulletProjectile* Bullet = SpawnProjectile(
		Cast<APawn>(GetOwner()),
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

	// 사격 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1000, 5.0f, FColor::Red, FString::Printf(TEXT("Fire - %d / %d"), RangeWeaponInstance->GetCurrentAmmo(), RangeWeaponInstance->GetMagazineSize()));

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

bool URangeWeaponHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(URangeWeaponInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool URangeWeaponHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool URangeWeaponHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool URangeWeaponHandlerComponent::CanUnequip() const
{
	if (!Super::CanUnequip())
	{
		return false;
	}

	// 사용 중
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		return false;
	}

	return true;
}

bool URangeWeaponHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return false;
	}

	return true;
}

bool URangeWeaponHandlerComponent::CanFire() const
{
	// 사용 불가
	if (!CanUse())
	{
		return false;
	}

	// 탄약 부족
	if (RangeWeaponInstance->GetCurrentAmmo() <= 0)
	{
		return false;
	}

	// 사격 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
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

	// 탄창 가득찬 경우
	if (RangeWeaponInstance->GetCurrentAmmo() == RangeWeaponInstance->GetMagazineSize())
	{
		return false;
	}

	// 재장전 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
	{
		return false;
	}

	// 여분 탄약 등, 외부 조건 확인
	if (!CanReloadDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 불가 - CanReloadDelegate is not Bound"));
		return false;
	}
	if (!CanReloadDelegate.Execute(RangeWeaponInstance))
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] GetMuzzleLocation 기본값 반환 - 메시에 %s 소켓이 없음 (장비=%s"), *MuzzleSocketName.ToString(), *GetNameSafe(RangeWeaponInstance));
	}

	return MuzzleLocation;
}

FRotator URangeWeaponHandlerComponent::GetMuzzleRotation() const
{
	FRotator MuzzleRotation = GetOwner()->GetActorRotation();
	if (EquipMeshComponent && EquipMeshComponent->DoesSocketExist(MuzzleSocketName))
	{
		MuzzleRotation = EquipMeshComponent->GetSocketRotation(MuzzleSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] GetMuzzleRotation 기본값 반환 - 메시에 %s 소켓이 없음 (장비=%s"), *MuzzleSocketName.ToString(), *GetNameSafe(RangeWeaponInstance));
	}

	return MuzzleRotation;
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
	APawn* Instigator,
	const FVector& StartLocation,
	const FRotator& Rotation)
{
	// 등록된 장비 없음
	if (!RangeWeaponInstance)
	{
		return nullptr;
	}

	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		return nullptr;
	}

	// 총알 액터 생성
	ABulletProjectile* BulletActor = GetWorld()->SpawnActor<ABulletProjectile>(
		RangeWeaponData->BulletClass,
		StartLocation,
		Rotation);
	if (!BulletActor)
	{
		return nullptr;
	}

	// 총알 초기 설정
	BulletActor->Initialize(
		Instigator,
		RangeWeaponData->Damage,
		RangeWeaponData->ProjectileSpeed * Rotation.Vector(),
		RangeWeaponData->ProjectileGravityScale,
		RangeWeaponData->ProjectileRange / RangeWeaponData->ProjectileSpeed);

	return BulletActor;
}

void URangeWeaponHandlerComponent::StartFireTimer()
{
	// 데이터 유효성 검증
	const FRangeWeaponDataRow* RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();
	if (!RangeWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 사격 타이머 활성화 실패 - 유효하지 않은 RangeWeaponData"));
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
	if (!RangeWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 타이머 활성화 실패 - 유효하지 않은 RangeWeaponData"));
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
	// 장비 메시 컴포넌트 확인
	if (!EquipMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 사격 애니메이션 재생 실패 - 유효하지 않은 EquipMeshComponent"));
		return;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 사격 애니메이션 재생 실패 - 유효하지 않은 EquippableItemData"));
		return;
	}

	// 장비 애니메이션 검증
	UEquipmentAnimationDataAsset* EquipmentAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!EquipmentAnimationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 사격 애니메이션 재생 실패 - 유효하지 않은 EquipmentAnimationData"));
		return;
	}

	// 사격 애니메이션 검증
	UAnimMontage* FireAnim = EquipmentAnimationData->WeaponFire;
	if (!FireAnim)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 사격 애니메이션 재생 실패 - 유효하지 않은 FireAnim"));
		return;
	}

	EquipMeshComponent->PlayAnimation(FireAnim, false);
}

void URangeWeaponHandlerComponent::PlayReloadAnimation()
{
	// 장비 메시 컴포넌트 확인
	if (!EquipMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 애니메이션 재생 실패 - 유효하지 않은 EquipMeshComponent"));
		return;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = RangeWeaponInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 애니메이션 재생 실패 - 유효하지 않은 EquippableItemData"));
		return;
	}

	// 장비 애니메이션 검증
	UEquipmentAnimationDataAsset* EquipmentAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!EquipmentAnimationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 애니메이션 재생 실패 - 유효하지 않은 EquipmentAnimationData"));
		return;
	}

	// 재장전 애니메이션 검증 - TODO: 조준 여부에 따라 구분
	UAnimMontage* WeaponReloadHip = EquipmentAnimationData->WeaponReloadHip;
	if (!WeaponReloadHip)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 애니메이션 재생 실패 - 유효하지 않은 WeaponReloadHip"));
		return;
	}

	EquipMeshComponent->PlayAnimation(WeaponReloadHip, false);
}

void URangeWeaponHandlerComponent::StopReloadAnimation()
{
	// 장비 메시 컴포넌트 확인
	if (!EquipMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 애니메이션 정지 실패 - 유효하지 않은 EquipMeshComponent"));
		return;
	}

	EquipMeshComponent->Stop();
}

void URangeWeaponHandlerComponent::OnReloadStarted()
{
	// 재장전 애니메이션 재생
	PlayReloadAnimation();

	// 재장전 타이머 활성화
	StartReloadTimer();

	// 재장전 시작 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1001, 5.0f, FColor::Red, FString::Printf(TEXT("Reload Started")));
}

void URangeWeaponHandlerComponent::OnReloadCompleted()
{
	// RequestReloadAmmoDelegate 바인딩 확인
	if (!RequestReloadAmmoDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 실패 - RequestReloadAmmoDelegate is not Bound"));
		return;
	}

	// 추가할 탄약 개수
	const int32 AddedAmmo = RequestReloadAmmoDelegate.Execute(RangeWeaponInstance);

	// 탄약 추가
	RangeWeaponInstance->AddAmmo(AddedAmmo);

	// 재장전 완료 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1001, 5.0f, FColor::Red, FString::Printf(TEXT("Reload Completed")));
}

void URangeWeaponHandlerComponent::OnReloadInterrupted()
{
	// 재장전 중이 아닌 경우
	if (!GetWorld() || !GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle))
	{
		return;
	}

	// 재장전 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

	// 재장전 애니메이션 중단
	StopReloadAnimation();

	// 재장전 취소 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1001, 5.0f, FColor::Red, FString::Printf(TEXT("Reload Interrupted")));
}
