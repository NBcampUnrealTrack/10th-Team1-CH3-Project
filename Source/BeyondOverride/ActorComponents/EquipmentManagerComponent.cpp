#include "ActorComponents/EquipmentManagerComponent.h"

#include "ActorComponents/MeleeWeaponHandlerComponent.h"
#include "ActorComponents/RangeWeaponHandlerComponent.h"
#include "ActorComponents/ThrowableItemHandlerComponent.h"
#include "ActorComponents/UtilityItemHandlerComponent.h"
#include "Enums/EquipmentSlot.h"
#include "Factory/ItemFactory.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Items/Objects/ThrowableItemInstance.h"
#include "Items/Objects/UtilityItemInstance.h"

UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActiveSlot = EEquipmentSlot::Unarmed;

	EquipmentHandlerComponents.Add(EEquipmentSlot::Unarmed, CreateDefaultSubobject<UMeleeWeaponHandlerComponent>(TEXT("Unarmed Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Primary, CreateDefaultSubobject<URangeWeaponHandlerComponent>(TEXT("Primary RangeWeapon Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Secondary, CreateDefaultSubobject<URangeWeaponHandlerComponent>(TEXT("Secondary RangeWeapon Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Melee, CreateDefaultSubobject<UMeleeWeaponHandlerComponent>(TEXT("MeleeWeapon Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Throwable, CreateDefaultSubobject<UThrowableItemHandlerComponent>(TEXT("ThrowableItem Handler Component")));
	EquipmentHandlerComponents.Add(EEquipmentSlot::Effect, CreateDefaultSubobject<UUtilityItemHandlerComponent>(TEXT("UtilityItem Handler Component")));
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

void UEquipmentManagerComponent::Initialize()
{
	// Unarmed 설정
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Unarmed))
	{
		// UNARM 아이템 생성 후 Unarmed 슬롯에 할당
		UItemInstanceBase* ItemInstance = FItemFactory::CreateItemInstance(GetOwner(), FName("UNARM"));
		if (UEquippableItemInstance* EquippableItemInstance = Cast<UEquippableItemInstance>(ItemInstance))
		{
			ActiveSlot = EEquipmentSlot::Unarmed;
			EquipmentHandlerComponents[ActiveSlot]->Assign(EquippableItemInstance);
			EquipmentHandlerComponents[ActiveSlot]->Equip();
			OnActiveSlotChangedDelegate.Broadcast(ActiveSlot, EquippableItemInstance);
			GEngine->AddOnScreenDebugMessage(100, 1000.f, FColor::Yellow, FString::Printf(TEXT("현재 슬롯 - %s"), *UEnum::GetValueAsString(ActiveSlot)));
		}
	}

	// 각 장비 핸들러의 델리게이트 연결
	BindDelegates();
}

bool UEquipmentManagerComponent::Equip(EEquipmentSlot Slot)
{
	// 유효하지 않은 슬롯
	if (!EquipmentHandlerComponents.Contains(Slot) || !EquipmentHandlerComponents[Slot])
	{
		return false;
	}

	// 이미 활성화된 슬롯인 경우 유지
	if (ActiveSlot == Slot)
	{
		return false;
	}

	// 현재 슬롯에 장착된 장비 존재 & 해제 실패 - 슬롯 유지
	if (EquipmentHandlerComponents.Contains(ActiveSlot) && EquipmentHandlerComponents[ActiveSlot]->GetEquippableItemInstance())
	{
		if (!EquipmentHandlerComponents[ActiveSlot]->Unequip())
		{
			return false;
		}
	}

	// 장비가 없는 슬롯으로 전환 - 비무장(Unarmed) 슬롯 전환
	if (!EquipmentHandlerComponents[Slot]->GetEquippableItemInstance())
	{
		Slot = EEquipmentSlot::Unarmed;
	}

	GEngine->AddOnScreenDebugMessage(100, 1000.f, FColor::Yellow, FString::Printf(TEXT("현재 슬롯 - %s"), *UEnum::GetValueAsString(Slot)));

	// 현재 장비 해제 성공 - 슬롯 전환
	ActiveSlot = Slot;
	EquipmentHandlerComponents[ActiveSlot]->Equip();

	// 장비 인스턴스
	UEquippableItemInstance* EquippableItemInstance = EquipmentHandlerComponents[Slot]->GetEquippableItemInstance();

	// OnActiveSlotChangedDelegate 송출 - 현재 활성화된 슬롯과 장비 전달
	OnActiveSlotChangedDelegate.Broadcast(ActiveSlot, EquippableItemInstance);

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
	return EquipmentHandlerComponents[ActiveSlot]->Use();
}

void UEquipmentManagerComponent::StartAction()
{
	if (EquipmentHandlerComponents.Contains(ActiveSlot) && EquipmentHandlerComponents[ActiveSlot])
	{
		EquipmentHandlerComponents[ActiveSlot]->StartAction();
	}
}

void UEquipmentManagerComponent::EndAction()
{
	if (EquipmentHandlerComponents.Contains(ActiveSlot) && EquipmentHandlerComponents[ActiveSlot])
	{
		EquipmentHandlerComponents[ActiveSlot]->EndAction();
	}
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
	// 현재 비무장이면, 장비가 등록된 슬롯으로 자동 전환 (비활성화)
	// else if (ActiveSlot == EEquipmentSlot::Unarmed)
	// {
	//     Equip(Slot);
	// }

	return true;
}

UItemInstanceBase* UEquipmentManagerComponent::Unassign(EEquipmentSlot Slot)
{
	// 유효하지 않은 슬롯
	if (!EquipmentHandlerComponents.Contains(Slot))
	{
		return nullptr;
	}

	// 비무장 제거 불가
	if (Slot == EEquipmentSlot::Unarmed)
	{
		return nullptr;
	}

	// 장비 제거
	UItemInstanceBase* ItemInstance = EquipmentHandlerComponents[Slot]->Unassign();

	// 장비 제거 실패
	if (!ItemInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] Unassign 실패 - %s 슬롯의 장비 제거 실패"), *UEnum::GetValueAsString(Slot));
		return nullptr;
	}

	// 비무장 슬롯 전환
	if (ActiveSlot == Slot)
	{
		Equip(EEquipmentSlot::Unarmed);
	}

	// 제거한 장비 반환
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

	// Throwable
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Throwable))
	{
		if (UThrowableItemHandlerComponent* ThrowableItemHandler = Cast<UThrowableItemHandlerComponent>(EquipmentHandlerComponents[EEquipmentSlot::Throwable]))
		{
			ThrowableItemHandler->OnCountUpdatedDelegate.AddUObject(this, &UEquipmentManagerComponent::OnEquipmentCountUpdated);
		}
	}

	// Effect
	if (EquipmentHandlerComponents.Contains(EEquipmentSlot::Effect))
	{
		if (UUtilityItemHandlerComponent* UtilityItemHandler = Cast<UUtilityItemHandlerComponent>(EquipmentHandlerComponents[EEquipmentSlot::Effect]))
		{
			UtilityItemHandler->OnCountUpdatedDelegate.AddUObject(this, &UEquipmentManagerComponent::OnEquipmentCountUpdated);
			UtilityItemHandler->OnEffectAppliedDelegate.AddUObject(this, &UEquipmentManagerComponent::OnEffectApplied);
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

void UEquipmentManagerComponent::OnEquipmentCountUpdated(UEquippableItemInstance* EquippableItemInstance)
{
	if (!OnEquipmentStackCountUpdatedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장비 개수 변경 이벤트 송출 실패 - OnEquipmentCountUpdatedDelegate is not Bound"));
	}

	UE_LOG(LogTemp, Warning, TEXT("[UEquipmentManagerComponent] 장비 개수 변경 - %s: %d"), *GetNameSafe(EquippableItemInstance), EquippableItemInstance->GetStackCount());

	// 슬롯과 장비 인스턴스 송출
	if (EquippableItemInstance->IsA(UThrowableItemInstance::StaticClass()))
	{
		OnEquipmentStackCountUpdatedDelegate.Broadcast(EEquipmentSlot::Throwable, EquippableItemInstance);
		// 개수 0일면 자동 제거
		if (EquippableItemInstance->GetStackCount() <= 0)
		{
			Unassign(EEquipmentSlot::Throwable);
		}
	}
	else if (EquippableItemInstance->IsA(UUtilityItemInstance::StaticClass()))
	{
		OnEquipmentStackCountUpdatedDelegate.Broadcast(EEquipmentSlot::Effect, EquippableItemInstance);
		if (EquippableItemInstance->GetStackCount() <= 0)
		{
			Unassign(EEquipmentSlot::Effect);
		}
	}

	// TODO: 캐릭터에서 해당 델리게이트 바인딩. 개수가 0개면 제거 수행
}

void UEquipmentManagerComponent::OnEffectApplied(UUtilityItemInstance* UtilityItemInstance) const
{
	OnEffectAppliedDelegate.Broadcast(UtilityItemInstance);
	// TODO: 캐릭터에서 전달받은 아이템에 따라 효과 적용
}
