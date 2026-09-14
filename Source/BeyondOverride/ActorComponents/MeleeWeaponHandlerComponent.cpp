#include "ActorComponents/MeleeWeaponHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "GameFramework/Character.h"
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
	// 해제 불가
	if (!CanUnequip())
	{
		return false;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = MeleeWeaponInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Unequip 실패 - %s: 유효하지 않은 EquippableItemData"), *GetNameSafe(MeleeWeaponInstance))
		return false;
	}

	// 캐릭터 메시 확인
	ACharacter* Character = GetOwner<ACharacter>();
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Unequip 실패 - Owner가 Character가 아님 (Owner=%s)"), *GetNameSafe(GetOwner()))
		return false;
	}

	USkeletalMeshComponent* CharacterMeshComponent = Character->GetMesh();
	if (!CharacterMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Unequip 실패 - Character가 SkeletalMeshComponent를 갖지 않음 (Character=%s)"), *GetNameSafe(Character))
		return false;
	}

	// 보관 소켓에 메시 부착
	const FName HolsterSocketName = EquippableItemData->HolsterSocketName;
	if (CharacterMeshComponent->DoesSocketExist(HolsterSocketName))
	{
		EquipMeshComponent->AttachToComponent( // 소켓에 부착
			CharacterMeshComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			HolsterSocketName);
	}
	// 보관 소켓이 없는 경우 메시 제거
	else
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Use()
{
	return false;
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
	return true;
}

bool UMeleeWeaponHandlerComponent::CanUse() const
{
	return true;
}
