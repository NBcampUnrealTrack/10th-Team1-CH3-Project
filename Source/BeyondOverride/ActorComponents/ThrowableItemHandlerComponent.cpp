#include "ActorComponents/ThrowableItemHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/ThrowableItemDataRow.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/ThrowableItemInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Throwables/ThrowableBase.h"

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

	// Throwable Item 데이터 저장
	ThrowableItemData = ThrowableItemInstance->GetThrowableItemData();

	// 유효하지 않은 데이터
	if (!ThrowableItemData)
	{
		ThrowableItemInstance = nullptr;
		return false;
	}

	// Assign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4000, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Assigned - %s"), *GetNameSafe(EquippableItemInstance)));

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

	// Throwable Item 데이터 제거
	ThrowableItemData = nullptr;

	// Unassign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4000, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Unassigned - %s"), *GetNameSafe(EquippableItemInstance)));

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UThrowableItemHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	// Equip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4001, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Equipped - %s"), *GetNameSafe(EquippableItemInstance)));

	return true;
}

bool UThrowableItemHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	// Unequip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4001, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Unequipped - %s"), *GetNameSafe(EquippableItemInstance)));

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

void UThrowableItemHandlerComponent::StartAction()
{
}

void UThrowableItemHandlerComponent::EndAction()
{
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
	if (!HasEquipment())
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
	if (!HasEquipment())
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

	// 투척 시작 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4002, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Started - %s"), *GetNameSafe(EquippableItemInstance)));
}

void UThrowableItemHandlerComponent::Throw()
{
	// 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(ThrowTimerHandle);

	// 투사체 액터 소환
	AThrowableBase* Throwable = SpawnThrowable();
	if (!Throwable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UThrowableItemHandlerComponent] 투척 실패 - %s의 투척 액터 생성 실패"), *GetNameSafe(ThrowableItemInstance));
		return;
	}

	// 사용 후 개수 차감
	const int Count = ThrowableItemInstance->GetStackCount();
	ThrowableItemInstance->SetStackCount(Count - 1);

	// 사용 후 개수 변경 델리게이트 송출
	OnCountUpdatedDelegate.Broadcast(ThrowableItemInstance);

	// 투척 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(4002, 5.0f, FColor::Orange, FString::Printf(TEXT("Throwable Item Throwed - %s"), *GetNameSafe(EquippableItemInstance)));
}

AThrowableBase* UThrowableItemHandlerComponent::SpawnThrowable()
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return nullptr;
	}

	const FVector ThrowStartLocation = GetThrowStartLocation();
	const FRotator ThrowRotation = GetAimRotation();

	// 소환 인자 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 투척 액터 생성
	AThrowableBase* ThrowableActor = GetWorld()->SpawnActor<AThrowableBase>(
		ThrowableItemData->ThrowableClass,
		ThrowStartLocation,
		FRotator::ZeroRotator,
		SpawnParams);
	if (!IsValid(ThrowableActor))
	{
		return nullptr;
	}

	// 투척 액터 던지기
	ThrowableActor->Throw(Cast<APawn>(GetOwner()), ThrowRotation, ThrowableItemData->ThrowForce);

	return ThrowableActor;
}
