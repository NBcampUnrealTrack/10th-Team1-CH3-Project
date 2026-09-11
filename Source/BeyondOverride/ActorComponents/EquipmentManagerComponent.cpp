#include "ActorComponents/EquipmentManagerComponent.h"

#include "ActorComponents/RangeWeaponHandlerComponent.h"
#include "Enums/EquipmentSlot.h"
#include "Items/Objects/EquippableItemInstance.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveSlot = EEquipmentSlot::Primary;

	EquipmentHandlerComponents.Add(EEquipmentSlot::Primary, CreateDefaultSubobject<URangeWeaponHandlerComponent>(TEXT("Primary RangeWeapon Handler Component")));
}

void UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
}

void UEquipmentManagerComponent::Unequip()
{
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

	if (!EquipmentHandlerComponents[Slot]->Assign(EquippableItemInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 실패 - %s -> %s Handler 등록 실패"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 등록 성공 - %s를 %s 슬롯에 등록 성공"), *GetNameSafe(ItemInstanceBase), *UEnum::GetValueAsString(Slot));
}

void UEquipmentManagerComponent::Unassign(EEquipmentSlot Slot)
{
}
