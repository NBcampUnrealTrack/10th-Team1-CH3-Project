#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "DataAssets/EquipmentAnimationDataAsset.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/MeleeWeaponDataRow.h"
#include "Items/Objects/MeleeWeaponInstance.h"

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
	return Super::CanUnequip();
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
