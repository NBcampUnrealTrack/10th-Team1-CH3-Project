#include "ActorComponents/EquipmentManagerComponent.h"

#include "ActorComponents/RangeWeaponHandlerComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "Enums/EquipmentSlot.h"
#include "GameFramework/Character.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Player/AnimInstance/BOAnimInstance.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveSlot = EEquipmentSlot::Primary;

	EquipmentHandlerComponents.Add(EEquipmentSlot::Primary, CreateDefaultSubobject<URangeWeaponHandlerComponent>(TEXT("Primary RangeWeapon Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Secondary, CreateDefaultSubobject<URangeWeaponHandlerComponent>(TEXT("Secondary RangeWeapon Handler Component")));
}

void UEquipmentManagerComponent::OnRegister()
{
	Super::OnRegister();

	// 장비 핸들러의 델리게이트 연결
	BindDelegates();
}

void UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(Slot))
		return;
	}

	// 활성화 슬롯 전환
	ActiveSlot = Slot;
	EquipmentHandlerComponents[Slot]->Equip();

	// TODO: 전환한 슬롯에 장비가 없으면, Unarmed 상태로 전환

	// 장비 인스턴스 확인
	UEquippableItemInstance* EquippableItemInstance = EquipmentHandlerComponents[Slot]->GetEquippableItemInstance();
	if (!EquippableItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: 등록된 장비가 없음"), *UEnum::GetValueAsString(Slot))
		return;
	}

	// 장비 데이터 확인
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: %s 장비의 EquippableItemData가 유효하지 않음"), *UEnum::GetValueAsString(Slot), *GetNameSafe(EquippableItemInstance))
		return;
	}

	// 장비 애니메이션 데이터 확인
	const UEquipmentAnimationDataAsset* WeaponAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!WeaponAnimationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: %s 장비의 WeaponAnimationData가 유효하지 않음"), *UEnum::GetValueAsString(Slot), *GetNameSafe(EquippableItemInstance))
		return;
	}

	// UBOAnimInstance 확인 - TODO: 결합도 낮추는 방향으로 리팩토링 필요
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			if (UBOAnimInstance* BOAnimInstance = Cast<UBOAnimInstance>(AnimInstance))
			{
				BOAnimInstance->ApplyEquipmentAnimation(WeaponAnimationData);
			}
		}
	}
}

void UEquipmentManagerComponent::Unequip()
{
	// 장비 애니메이션 해제 (기본 애니메이션)
	// TODO: ABOAnimInstance에 기본 애니메이션 등록 후 함수 호출하여 기본 애니메이션으로 복구

	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Unequip 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(ActiveSlot))
		return;
	}

	// 장비 해제
	EquipmentHandlerComponents[ActiveSlot]->Unequip();
}

void UEquipmentManagerComponent::Use()
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Use 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(ActiveSlot))
		return;
	}

	// 장비 사용
	EquipmentHandlerComponents[ActiveSlot]->Use();
}

void UEquipmentManagerComponent::Reload()
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		return;
	}

	// RangeWeapon이 아닌 경우
	URangeWeaponHandlerComponent* RangeWeaponHandler = Cast<URangeWeaponHandlerComponent>(EquipmentHandlerComponents[ActiveSlot]);
	if (!RangeWeaponHandler)
	{
		return;
	}

	// 재장전
	const bool bSucceed = RangeWeaponHandler->Reload();
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 실패 - %s 슬롯: %s"), *UEnum::GetValueAsString(ActiveSlot), *GetNameSafe(RangeWeaponHandler->GetEquippableItemInstance()));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 성공 - %s 슬롯: %s"), *UEnum::GetValueAsString(ActiveSlot), *GetNameSafe(RangeWeaponHandler->GetEquippableItemInstance()));
}

void UEquipmentManagerComponent::Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase)
{
	// 장착 불가능한 타입
	UEquippableItemInstance* EquippableItemInstance = Cast<UEquippableItemInstance>(ItemInstanceBase);
	if (!EquippableItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 실패 - %s -> EquippableItemInstance 캐스팅 실패"), *GetNameSafe(ItemInstanceBase));
		return;
	}

	// 슬롯이 없음
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(Slot))
		return;
	}

	// 등록 시도
	const bool bSucceed = EquipmentHandlerComponents[Slot]->Assign(EquippableItemInstance);
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 실패 - %s -> %s Handler 등록 실패"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 성공 - %s를 %s 슬롯에 등록 성공"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));
}

void UEquipmentManagerComponent::Unassign(EEquipmentSlot Slot)
{
	if (EquipmentHandlerComponents.Contains(Slot))
	{
		EquipmentHandlerComponents[Slot]->Unassign();
	}
}

void UEquipmentManagerComponent::BindDelegates()
{
	// Primary (Range Weapon)
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Primary))
	{
		if (URangeWeaponHandlerComponent* PrimaryRangeWeaponHandler = Cast<URangeWeaponHandlerComponent>(EquipmentHandlerComponents[EEquipmentSlot::Primary]))
		{
			PrimaryRangeWeaponHandler->CanReloadDelegate.BindUObject(this, &UEquipmentManagerComponent::OnCanReload);
			PrimaryRangeWeaponHandler->RequestReloadAmmoDelegate.BindUObject(this, &UEquipmentManagerComponent::OnRequestReloadAmmo);
		}
	}

	//  Secondary (Range Weapon)
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Secondary))
	{
		if (URangeWeaponHandlerComponent* SecondaryRangeWeaponHandler = Cast<URangeWeaponHandlerComponent>(EquipmentHandlerComponents[EEquipmentSlot::Secondary]))
		{
			SecondaryRangeWeaponHandler->CanReloadDelegate.BindUObject(this, &UEquipmentManagerComponent::OnCanReload);
			SecondaryRangeWeaponHandler->RequestReloadAmmoDelegate.BindUObject(this, &UEquipmentManagerComponent::OnRequestReloadAmmo);
		}
	}
}

bool UEquipmentManagerComponent::OnCanReload(URangeWeaponInstance* RangeWeaponInstance) const
{
	if (!CanReloadDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 불가 - CanReloadDelegate is not Bound"));
		return false;
	}

	// 재장전 가능 여부 반환
	return CanReloadDelegate.Execute(RangeWeaponInstance);
}

int32 UEquipmentManagerComponent::OnRequestReloadAmmo(URangeWeaponInstance* RangeWeaponInstance) const
{
	if (!RequestReloadAmmoDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 실패 - RequestReloadAmmoDelegate is not Bound"));
		return 0;
	}

	// 재장전에 사용할 탄약 개수 전달
	return RequestReloadAmmoDelegate.Execute(RangeWeaponInstance);
}
