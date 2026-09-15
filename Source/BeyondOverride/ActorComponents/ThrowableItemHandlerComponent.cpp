#include "ActorComponents/ThrowableItemHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/ThrowableItemDataRow.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/ThrowableItemInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectiles/Throwables/ThrowableProjectile.h"

UThrowableItemHandlerComponent::UThrowableItemHandlerComponent()
{
	ThrowableItemInstance = nullptr;
}

bool UThrowableItemHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Throwable Item 인스턴스 저장
	ThrowableItemInstance = Cast<UThrowableItemInstance>(EquippableItemInstance);

	// 등록 성공
	return true;
}

UEquippableItemInstance* UThrowableItemHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Throwable Item 인스턴스 제거
	ThrowableItemInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UThrowableItemHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::Use()
{
	if (!CanThrow())
	{
		return false;
	}

	// 투척 시작
	StartThrow();

	return true;
}

bool UThrowableItemHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(UThrowableItemInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UThrowableItemHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UThrowableItemHandlerComponent::CanUnequip() const
{
	if (!Super::CanUnequip())
	{
		return false;
	}

	// 투척 중
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ThrowTimerHandle))
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FThrowableItemDataRow* ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();
	if (!ThrowableItemData)
	{
		return false;
	}

	return true;
}

bool UThrowableItemHandlerComponent::CanThrow() const
{
	if (!CanUse())
	{
		return false;
	}

	// 투척 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(ThrowTimerHandle))
	{
		return false;
	}

	return true;
}

FRotator UThrowableItemHandlerComponent::GetAimRotation() const
{
	// 액터 방향
	const FRotator ActorRotation = GetOwner()->GetActorRotation();

	// Owner 유효성 검증
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return ActorRotation;
	}

	// 컨트롤러 유효성 검증
	AController* Controller = Pawn->GetController();
	if (!Controller)
	{
		return ActorRotation;
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
	const FVector EndLocation = StartLocation + ViewRotation.Vector() * 2000.f; // 20m

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

	// 투척 방향 구하기
	const FRotator ThrowRotation = UKismetMathLibrary::FindLookAtRotation(
		GetThrowStartLocation(),
		AimLocation);

	return ThrowRotation;
}

FVector UThrowableItemHandlerComponent::GetThrowStartLocation() const
{
	FVector SocketLocation = GetOwner()->GetActorLocation();

	// 등록된 장비 없음
	if (!ThrowableItemInstance)
	{
		return SocketLocation;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = ThrowableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		return SocketLocation;
	}

	// 소켓 위치 구하기
	const FName EquipSocketName = EquippableItemData->EquipSocketName; // 장착 소켓 이름
	if (EquipMeshComponent && EquipMeshComponent->DoesSocketExist(EquipSocketName))
	{
		SocketLocation = EquipMeshComponent->GetSocketLocation(EquipSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UThrowableItemHandlerComponent] GetThrowStartLocation 기본값 반환 - 메시에 %s 소켓이 없음 (장비=%s"), *EquipSocketName.ToString(), *GetNameSafe(ThrowableItemInstance));
	}

	return SocketLocation;
}

void UThrowableItemHandlerComponent::StartThrow()
{
	// 등록된 장비 없음
	if (!ThrowableItemInstance)
	{
		return;
	}

	// 데이터 유효성 검증
	const FThrowableItemDataRow* ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();
	if (!ThrowableItemData)
	{
		return;
	}

	// 투척 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		ThrowTimerHandle,
		this,
		&UThrowableItemHandlerComponent::Throw,
		ThrowableItemData->ThrowDuration,
		false);
}

void UThrowableItemHandlerComponent::Throw()
{
	AThrowableProjectile* Throwable = SpawnThrowable();
	if (!Throwable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UThrowableItemHandlerComponent] 투척 실패 - %s의 투척 액터 생성 실패"), *GetNameSafe(ThrowableItemInstance));
	}
}

AThrowableProjectile* UThrowableItemHandlerComponent::SpawnThrowable()
{
	// 등록된 장비 없음
	if (!ThrowableItemInstance)
	{
		return nullptr;
	}

	// 데이터 유효성 검증
	const FThrowableItemDataRow* ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();
	if (!ThrowableItemData)
	{
		return nullptr;
	}

	const FVector ThrowStartLocation = GetThrowStartLocation();
	const FRotator ThrowRotation = GetAimRotation();

	// 투척 액터 생성
	AThrowableProjectile* ThrowableActor = GetWorld()->SpawnActor<AThrowableProjectile>(
		ThrowableItemData->ThrowableClass,
		ThrowStartLocation,
		FRotator::ZeroRotator);
	if (!ThrowableActor)
	{
		return nullptr;
	}

	// 투척 초기 설정
	ThrowableActor->Initalize(
		Cast<APawn>(GetOwner()),
		ThrowableItemData->Damage,
		ThrowableItemData->Radius,
		ThrowableItemData->ActivationDelay,
		ThrowableItemData->ThrowSpeed * ThrowRotation.Vector(),
		ThrowableItemData->ThrowGravityScale);

	return ThrowableActor;
}
