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
}

void UEquipmentManagerComponent::Use()
{
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
