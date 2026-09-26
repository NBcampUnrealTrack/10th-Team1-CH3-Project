#include "ActorComponents/RangeWeaponHandlerComponent.h"

#include "Animation/AnimInstance.h"
#include "Bullets/BulletBase.h"
#include "DataAssets/EquipmentAnimationDataAsset.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/RangeWeaponDataRow.h"
#include "Enums/FireMode.h"
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

	// 사격 활성화 여부
	bIsActive = false;
	// 조준 여부
	bIsAiming = false;
}

void URangeWeaponHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 타임라인 진행
	RecoilPitchTimeline.TickTimeline(DeltaTime);
	RecoilYawTimeline.TickTimeline(DeltaTime);
	SpreadDegreeTimeline.TickTimeline(DeltaTime);

	// 적용할 반동 값 계산 (누적값이 클수록 큰 변화량)
	FVector2D RecoilDelta(
		FMath::FInterpConstantTo(0, RecoilAccumulator.X, DeltaTime, FMath::Max(20 * FMath::Abs(RecoilAccumulator.X), 5)),
		FMath::FInterpConstantTo(0, RecoilAccumulator.Y, DeltaTime, FMath::Max(20 * FMath::Abs(RecoilAccumulator.Y), 5)));

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

float URangeWeaponHandlerComponent::GetCurrentSpreadDegree() const
{
	float SpreadDegree = 0.f;
	if (UCurveFloat* SpreadCurve = RangeWeaponData->SpreadCurve)
	{
		SpreadDegree = SpreadCurve->GetFloatValue(SpreadDegreeTimeline.GetPlaybackPosition());
	}

	return SpreadDegree;
}

bool URangeWeaponHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Range Weapon 인스턴스 저장
	RangeWeaponInstance = Cast<URangeWeaponInstance>(EquippableItemInstance);

	// Range Weapon 데이터 저장
	RangeWeaponData = RangeWeaponInstance->GetRangeWeaponData();

	// 유효하지 않은 데이터
	if (!RangeWeaponData)
	{
		RangeWeaponInstance = nullptr;
		return false;
	}

	// 틱 활성화
	SetComponentTickEnabled(true);

	// 타임라인 설정
	SetupTimeline();

	// Assign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2000, 5.0f, FColor::Blue, FString::Printf(TEXT("Range Weapon Assigned - %s"), *GetNameSafe(EquippableItemInstance)));

	// 등록 성공
	return true;
}

UEquippableItemInstance* URangeWeaponHandlerComponent::Unassign()
{
	if (IsEquipping())
	{
		OnEquipInterrupted();
	}

	// 좌클릭 및 연사 종료
	EndAction();

	if (GetWorld())
	{
		// 기존 사격 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	// 장전 중이면 장전 취소
	OnReloadInterrupted();

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

	// Range Weapon 데이터 제거
	RangeWeaponData = nullptr;

	// Unassign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2000, 5.0f, FColor::Blue, FString::Printf(TEXT("Range Weapon Unassigned - %s"), *GetNameSafe(EquippableItemInstance)));

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool URangeWeaponHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	// 초기 조준 설정 - 비조준
	StopAiming();

	// Equip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2001, 5.0f, FColor::Blue, FString::Printf(TEXT("Range Weapon Equipped - %s"), *GetNameSafe(EquippableItemInstance)));

	return true;
}

bool URangeWeaponHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	//// 재장전 중이면 취소
	// OnReloadInterrupted();

	EndAction();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	// 조준 해제
	StopAiming();

	// Unequip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2001, 5.0f, FColor::Blue, FString::Printf(TEXT("Range Weapon Unequipped - %s"), *GetNameSafe(EquippableItemInstance)));

	return true;
}

bool URangeWeaponHandlerComponent::Use()
{
	if (!bIsActive)
	{
		StartAction();
	}

	return false;
}

void URangeWeaponHandlerComponent::StartAction()
{
	// 활성화 플래그 설정
	bIsActive = true;

	// 사격
	Fire();
}

void URangeWeaponHandlerComponent::EndAction()
{
	// 활성화 플래그 제거
	bIsActive = false;
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

void URangeWeaponHandlerComponent::StartAiming()
{
	bIsAiming = true;
}

void URangeWeaponHandlerComponent::StopAiming()
{
	bIsAiming = false;
}

bool URangeWeaponHandlerComponent::IsReloading() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ReloadTimerHandle);
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

	// 장전 중에는 해제 및 교체 불가
	if (IsReloading())
	{
		return false;
	}

	//// 사용 중
	// if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	//{
	//	return false;
	// }

	return true;
}

bool URangeWeaponHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	if (IsReloading())
	{
		return false;
	}

	return true;
}

void URangeWeaponHandlerComponent::OnEquipCompleted()
{
	Super::OnEquipCompleted();

	if (bIsActive)
	{
		Fire();
	}
}

void URangeWeaponHandlerComponent::Fire()
{
	// 사격 불가
	if (!CanFire())
	{
		return;
	}

	// 총알 소환
	SpawnBullets();

	// 반동 추가
	AddRecoil();

	// 사격 애니메이션 재생
	PlayFireAnimation();

	// 탄약 소모
	RangeWeaponInstance->ConsumeAmmo();

	// 사격 쿨다운 설정
	StartFireTimer();

	// 반동 & 탄 퍼짐 타임라인 재생
	PlayTimeline();

	// 사격 실행 델리게이트 송출
	OnFireExecutedDelegate.Broadcast();

	// 사격 후 탄약 개수 델리게이트 송출
	OnAmmoCountUpdatedDelegate.Broadcast(RangeWeaponInstance->GetCurrentAmmo());

	// 사격 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2002, 5.0f, FColor::Blue, FString::Printf(TEXT("Fire - %d / %d"), RangeWeaponInstance->GetCurrentAmmo(), RangeWeaponInstance->GetMagazineSize()));
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
	if (!HasEquipment())
	{
		return false;
	}

	// 장착 중인 경우
	if (IsEquipping())
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
	if (!CanReloadDelegate.Execute(RangeWeaponData->AmmoItemID))
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
	if (!HasEquipment())
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

void URangeWeaponHandlerComponent::PlayTimeline(bool bReverse)
{
	if (!bReverse)
	{
		RecoilPitchTimeline.Play();
		RecoilYawTimeline.Play();
		SpreadDegreeTimeline.Play();
	}
	else
	{
		RecoilPitchTimeline.Reverse();
		RecoilYawTimeline.Reverse();
		SpreadDegreeTimeline.Reverse();
	}
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
	if (!HasEquipment())
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
	// SpreadCurve 유효성 검증
	const UCurveFloat* SpreadCurve = RangeWeaponData->SpreadCurve;
	if (!SpreadCurve)
	{
		return AimRotation;
	}

	// 탄 퍼짐 각도
	float SpreadDegree = SpreadCurve->GetFloatValue(SpreadDegreeTimeline.GetPlaybackPosition());
	if (bIsAiming) // 조준 상태면 정확도 증가
	{
		SpreadDegree *= RangeWeaponData->AimSpreadMultiplier;
	}
	const float SpreadRadians = FMath::DegreesToRadians(SpreadDegree);

	// 원뿔 내 균일 분포
	return FMath::VRandCone(
			   AimRotation.Vector(),
			   SpreadRadians)
		.Rotation();
}

void URangeWeaponHandlerComponent::SpawnBullets()
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return;
	}

	const FVector MuzzleLocation = GetMuzzleLocation(); // 총구 위치
	const FRotator AimRotation = GetAimRotation();      // 목표 방향

	// 소환 인자 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 사격 당 소환할 개수만큼 반복
	for (int32 i = 0; i < RangeWeaponData->ProjectilesPerShot; ++i)
	{
		const FRotator SpreadRotation = GetSpreadRotation(AimRotation); // 랜덤 탄 퍼짐 적용된 방향

		// 총알 액터 생성
		ABulletBase* BulletActor = GetWorld()->SpawnActor<ABulletBase>(
			RangeWeaponData->BulletClass,
			MuzzleLocation,
			SpreadRotation,
			SpawnParams); // 소환 인자 지정
		if (!BulletActor)
		{
			continue;
		}

		// 총알 초기 설정
		BulletActor->Initialize(
			Cast<APawn>(GetOwner()),
			RangeWeaponData->Damage,
			RangeWeaponData->ProjectileSpeed * SpreadRotation.Vector(),
			RangeWeaponData->ProjectileGravityScale,
			RangeWeaponData->ProjectileRange / RangeWeaponData->ProjectileSpeed);
	}
}

void URangeWeaponHandlerComponent::StartFireTimer()
{
	// 사격 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&URangeWeaponHandlerComponent::OnFireCompleted,
		RangeWeaponData->FireRate,
		false);
}

void URangeWeaponHandlerComponent::StartReloadTimer()
{
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

void URangeWeaponHandlerComponent::OnFireCompleted()
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return;
	}

	// 타이머를 명시적으로 제거
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);

	// 사격 모드
	const EFireMode FireMode = RangeWeaponData->FireMode;

	// 활성화 & FullAuto -> 반복 사격
	if (CanFire() && bIsActive && FireMode == EFireMode::FullAuto)
	{
		// 사격
		Fire();
	}
	else
	{
		// 반동 & 탄 퍼짐 타임라인 역재생 - 회복
		PlayTimeline(true);
	}
}

void URangeWeaponHandlerComponent::OnReloadStarted()
{
	// 재장전 애니메이션 재생
	PlayReloadAnimation();

	// 재장전 타이머 활성화
	StartReloadTimer();

	// 재장전 시작 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2002, 5.0f, FColor::Blue, FString::Printf(TEXT("Reload Started - %s"), *GetNameSafe(EquippableItemInstance)));
}

void URangeWeaponHandlerComponent::OnReloadCompleted()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}

	// RequestReloadAmmoDelegate 바인딩 확인
	if (!RequestReloadAmmoDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[URangeWeaponHandlerComponent] 재장전 실패 - RequestReloadAmmoDelegate is not Bound"));
		return;
	}

	// 요청할 탄약 정보
	const FName AmmoItemID = RangeWeaponData->AmmoItemID;
	const int32 RequestedAmmoCount = RangeWeaponInstance->GetMagazineSize() - RangeWeaponInstance->GetCurrentAmmo();

	// 추가할 탄약 개수
	const int32 AddedAmmo = RequestReloadAmmoDelegate.Execute(RangeWeaponData->AmmoItemID, RequestedAmmoCount);

	// 탄약 추가
	RangeWeaponInstance->AddAmmo(AddedAmmo);

	// 재장전 후 탄약 개수 델리게이트 송출
	OnAmmoCountUpdatedDelegate.Broadcast(RangeWeaponInstance->GetCurrentAmmo());

	if (bIsActive)
	{
		Fire();
	}

	// 재장전 완료 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2002, 5.0f, FColor::Blue, FString::Printf(TEXT("Reload Completed - %s"), *GetNameSafe(EquippableItemInstance)));
}

void URangeWeaponHandlerComponent::OnReloadInterrupted()
{
	// 재장전 중이 아닌 경우
	if (!IsReloading())
	{
		return;
	}

	// 재장전 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

	// 재장전 애니메이션 중단
	StopReloadAnimation();

	// 캐릭터의 장전 몽타주 정지
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (Character && Character->GetMesh())
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.1f);
		}
	}

	// 재장전 중단 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(2002, 5.0f, FColor::Blue, FString::Printf(TEXT("Reload Interrupted - %s"), *GetNameSafe(EquippableItemInstance)));
}
