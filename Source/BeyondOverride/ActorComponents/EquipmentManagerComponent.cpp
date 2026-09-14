#include "ActorComponents/EquipmentManagerComponent.h"

#include "ActorComponents/MeleeWeaponHandlerComponent.h"
#include "ActorComponents/RangeWeaponHandlerComponent.h"
#include "Enums/EquipmentSlot.h"
#include "Factory/ItemFactory.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/ItemInstanceBase.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveSlot = EEquipmentSlot::Unarmed;

	EquipmentHandlerComponents.Add(EEquipmentSlot::Unarmed, CreateDefaultSubobject<UMeleeWeaponHandlerComponent>(TEXT("Unarmed Handler Component")));
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

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Unarmed 설정
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Unarmed))
	{
		// UNARM 아이템 생성 후 Unarmed 슬롯에 할당
		UItemInstanceBase* ItemInstance = FItemFactory::CreateItemInstance(GetOwner(), FName("UNARM"));
		if (UEquippableItemInstance* EquippableItemInstance = Cast<UEquippableItemInstance>(ItemInstance))
		{
			EquipmentHandlerComponents[EEquipmentSlot::Unarmed]->Assign(EquippableItemInstance);
			Equip(EEquipmentSlot::Unarmed);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Unarmed 설정 실패"));
}

bool UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
	// 유효하지 않은 슬롯
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Equip 실패 - %s 슬롯이 유효하지 않음"), *UEnum::GetValueAsString(Slot))
		return false;
	}

	GEngine->AddOnScreenDebugMessage(100, 1000.f, FColor::Yellow, FString::Printf(TEXT("현재 슬롯 - %s"), *UEnum::GetValueAsString(ActiveSlot)));

	// 이미 활성화된 슬롯인 경우 유지
	if (ActiveSlot == Slot)
	{
		return false;
	}

	// 현재 슬롯에 장착된 장비 존재 & 해제 실패 - 슬롯 유지
	if (EquipmentHandlerComponents[ActiveSlot]->GetEquippableItemInstance() && !EquipmentHandlerComponents[ActiveSlot]->Unequip())
	{
		return false;
	}

	GEngine->AddOnScreenDebugMessage(100, 1000.f, FColor::Yellow, FString::Printf(TEXT("현재 슬롯 - %s"), *UEnum::GetValueAsString(Slot)));

	// 현재 장비 해제 성공 - 슬롯 전환
	ActiveSlot = Slot;
	EquipmentHandlerComponents[ActiveSlot]->Equip();

	// 장비 인스턴스
	UEquippableItemInstance* EquippableItemInstance = EquipmentHandlerComponents[Slot]->GetEquippableItemInstance();

	// OnEquipmentChangedDelegate 송출
	OnEquipmentChangedDelegate.Broadcast(EquippableItemInstance);

	return true;
}

bool UEquipmentManagerComponent::Unequip()
{
	// 비무장 슬롯 전환
	return Equip(EEquipmentSlot::Unarmed);
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
		UE_LOG(LogTemp, Warning, TEXT("이미 활성화된 슬롯에 등록함"));
		EquipmentHandlerComponents[Slot]->Equip();
	}
	// 비활성화 슬롯에 장착 시 Unequip()
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("활성화 슬롯과 다른 슬롯에 등록함"));
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
