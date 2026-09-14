#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "CollisionShape.h"

#include "DataAssets/EquipmentAnimationDataAsset.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/MeleeWeaponDataRow.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Items/Objects/MeleeWeaponInstance.h"
#include "Kismet/GameplayStatics.h"

UMeleeWeaponHandlerComponent::UMeleeWeaponHandlerComponent()
{
	MeleeWeaponInstance = nullptr;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::GetEquippableItemInstance() const
{
	return MeleeWeaponInstance;
}

bool UMeleeWeaponHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Melee Weapon 인스턴스 저장
	MeleeWeaponInstance = Cast<UMeleeWeaponInstance>(EquippableItemInstance);

	// 등록 성공
	return true;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Melee Weapon 인스턴스 제거
	MeleeWeaponInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UMeleeWeaponHandlerComponent::Equip()
{
	if (!Equip())
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Use()
{
	if (!CanAttack())
	{
		return false;
	}

	// 근접무기 데이터
	const FMeleeWeaponDataRow* MeleeWeaponData = MeleeWeaponInstance->GetMeleeWeaponData();

	// 결과 값 저장 배열
	TArray<FOverlapResult> OverlapResults;

	// Query 설정
	FCollisionObjectQueryParams ObjectQueryParams;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	// 콜리전
	float AttackRadius = MeleeWeaponData->AttackRadius;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius / 2);

	// 콜리전 생성 및 오버랩 액터 확인
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetOwner()->GetActorLocation() + AttackRadius / 2 * GetOwner()->GetActorForwardVector(),
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape,
		QueryParams);

	// 공격 범위 내 액터에 데미지 적용
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();
		if (Actor && Actor != GetOwner())
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				MeleeWeaponData->Damage,
				GetOwner()->GetInstigatorController(),
				GetOwner(),
				UDamageType::StaticClass());
		}
	}

	// 공격 애니메이션 재생
	PlayAttackAnimation();

	// 공격 타이머 활성화
	StartAttackTimer();

	return true;
}

bool UMeleeWeaponHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(UMeleeWeaponInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UMeleeWeaponHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UMeleeWeaponHandlerComponent::CanUnequip() const
{
	if (!Super::CanUnequip())
	{
		return false;
	}

	// 공격 중
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FMeleeWeaponDataRow* RangeWeaponData = MeleeWeaponInstance->GetMeleeWeaponData();
	if (!RangeWeaponData)
	{
		return false;
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::CanAttack() const
{
	if (!CanUse())
	{
		return false;
	}

	// 공격 딜레이
	if (!GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(AttackTimerHandle))
	{
		return false;
	}

	return true;
}

void UMeleeWeaponHandlerComponent::StartAttackTimer()
{
	// 데이터 유효성 검증
	const FMeleeWeaponDataRow* MeleeWeaponData = MeleeWeaponInstance->GetMeleeWeaponData();
	if (!MeleeWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] 공격 타이머 활성화 실패 - 유효하지 않은 MeleeWeaponData"));
		return;
	}

	// 공격 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		MeleeWeaponData->AttackInterval,
		false);
}

void UMeleeWeaponHandlerComponent::PlayAttackAnimation()
{
	// 장비 메시 컴포넌트 확인
	if (!EquipMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] 공격 애니메이션 재생 실패 - 유효하지 않은 EquipMeshComponent"));
		return;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] 공격 애니메이션 재생 실패 - 유효하지 않은 EquippableItemData"));
		return;
	}

	// 장비 애니메이션 검증
	UEquipmentAnimationDataAsset* EquipmentAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!EquipmentAnimationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] 공격 애니메이션 재생 실패 - 유효하지 않은 EquipmentAnimationData"));
		return;
	}

	// 사격 애니메이션 검증
	UAnimMontage* FireAnim = EquipmentAnimationData->WeaponFire;
	if (!FireAnim)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] 공격 애니메이션 재생 실패 - 유효하지 않은 FireAnim"));
		return;
	}

	EquipMeshComponent->PlayAnimation(FireAnim, false);
}
