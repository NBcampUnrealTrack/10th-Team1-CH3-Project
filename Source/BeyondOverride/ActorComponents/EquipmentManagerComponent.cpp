#include "ActorComponents/EquipmentManagerComponent.h"

#include "ActorComponents/MeleeWeaponHandlerComponent.h"
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
	EquipmentHandlerComponents.Add(EEquipmentSlot::Melee, CreateDefaultSubobject<UMeleeWeaponHandlerComponent>(TEXT("MeleeWeapon Handler Component")));
}

EEquipmentSlot UEquipmentManagerComponent::GetActiveSlot() const
{
	return ActiveSlot;
}

bool UEquipmentManagerComponent::HasEquipment(EEquipmentSlot Slot) const
{
	if (!EquipmentHandlerComponents.Contains(Slot))
	{
		return false;
	}

	return EquipmentHandlerComponents[Slot]->GetEquippableItemInstance() != nullptr;
}

void UEquipmentManagerComponent::OnRegister()
{
	Super::OnRegister();

	// 장비 핸들러의 델리게이트 연결
	BindDelegates();
}

bool UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(Slot))
		return false;
	}

	// 이미 활성화된 슬롯인 경우 유지
	if (ActiveSlot == Slot)
	{
		return false;
	}

	// 이전 슬롯의 장비 해제
	if (EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		EquipmentHandlerComponents[ActiveSlot]->Unequip();
	}

	// 활성화 슬롯 전환 및 장비 장착
	if (!EquipmentHandlerComponents[Slot]->Equip())
	{
		return false;
	}
	ActiveSlot = Slot;

	// TODO: 전환한 슬롯에 장비가 없으면, Unarmed 상태로 전환

	// 장비 인스턴스 확인
	UEquippableItemInstance* EquippableItemInstance = EquipmentHandlerComponents[Slot]->GetEquippableItemInstance();
	if (!EquippableItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: 등록된 장비가 없음"), *UEnum::GetValueAsString(Slot))
		return false;
	}

	// 장비 데이터 확인
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: %s 장비의 EquippableItemData가 유효하지 않음"), *UEnum::GetValueAsString(Slot), *GetNameSafe(EquippableItemInstance))
		return false;
	}

	// 장비 애니메이션 데이터 확인
	const UEquipmentAnimationDataAsset* WeaponAnimationData = EquippableItemData->EquipmentAnimationData;
	if (!WeaponAnimationData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장착 실패 - %s 슬롯: %s 장비의 WeaponAnimationData가 유효하지 않음"), *UEnum::GetValueAsString(Slot), *GetNameSafe(EquippableItemInstance))
		return false;
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

	return true;
}

bool UEquipmentManagerComponent::Unequip()
{
	// 장비 애니메이션 해제 (기본 애니메이션)
	// TODO: ABOAnimInstance에 기본 애니메이션 등록 후 함수 호출하여 기본 애니메이션으로 복구

	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Unequip 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(ActiveSlot))
		return false;
	}

	// 장비 해제
	EquipmentHandlerComponents[ActiveSlot]->Unequip();

	return true;
}

bool UEquipmentManagerComponent::Use()
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Use 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(ActiveSlot))
		return false;
	}

	// 장비 사용
	EquipmentHandlerComponents[ActiveSlot]->Use();

	return true;
}

bool UEquipmentManagerComponent::Reload()
{
	// 슬롯 확인
	if (!EquipmentHandlerComponents.Contains(ActiveSlot))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Reload 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(ActiveSlot))
		return false;
	}

	// RangeWeapon이 아닌 경우
	URangeWeaponHandlerComponent* RangeWeaponHandler = Cast<URangeWeaponHandlerComponent>(EquipmentHandlerComponents[ActiveSlot]);
	if (!RangeWeaponHandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Reload 실패 - %s 슬롯이 URangeWeaponHandlerComponent가 아님"), *UEnum::GetValueAsString(ActiveSlot))
		return false;
	}

	// 재장전
	const bool bSucceed = RangeWeaponHandler->Reload();
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 실패 - %s 슬롯: %s"), *UEnum::GetValueAsString(ActiveSlot), *GetNameSafe(RangeWeaponHandler->GetEquippableItemInstance()));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 재장전 성공 - %s 슬롯: %s"), *UEnum::GetValueAsString(ActiveSlot), *GetNameSafe(RangeWeaponHandler->GetEquippableItemInstance()));
	return true;
}

bool UEquipmentManagerComponent::Assign(EEquipmentSlot Slot, UItemInstanceBase* ItemInstanceBase)
{
	// 장착 불가능한 타입
	UEquippableItemInstance* EquippableItemInstance = Cast<UEquippableItemInstance>(ItemInstanceBase);
	if (!EquippableItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Assign 실패 - %s -> EquippableItemInstance 캐스팅 실패"), *GetNameSafe(ItemInstanceBase));
		return false;
	}

	// 슬롯이 없음
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Assign 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(Slot))
		return false;
	}

	// 등록 시도
	const bool bSucceed = EquipmentHandlerComponents[Slot]->Assign(EquippableItemInstance);
	if (!bSucceed)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Assign 실패 - %s -> %s Handler 등록 실패"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Assign 성공 - %s를 %s 슬롯에 등록 성공"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));

	// 활성화 슬롯에 장착 시 Equip()
	if (Slot == ActiveSlot)
	{
		EquipmentHandlerComponents[Slot]->Equip();
	}
	// 비활성화 슬롯에 장착 시 Unequip()
	else
	{
		EquipmentHandlerComponents[Slot]->Unequip();
	}

	return true;
}

UItemInstanceBase* UEquipmentManagerComponent::Unassign(EEquipmentSlot Slot)
{
	if (!EquipmentHandlerComponents.Contains(Slot))
	{
		return nullptr;
	}

	UItemInstanceBase* ItemInstance = EquipmentHandlerComponents[Slot]->Unassign();
	if (!ItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Unassign 실패 - %s 슬롯의 장비 제거 실패"), *UEnum::GetValueAsString(Slot));
		return nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Unassign 성공 - 슬롯의 장비 제거 성공"), *UEnum::GetValueAsString(Slot));
	return ItemInstance;
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
