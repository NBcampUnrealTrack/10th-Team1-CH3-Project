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

bool UMeleeWeaponHandlerComponent::Assign(UEquippableItemInstance* EquippableItemInstance)
{
	// 이미 등록된 장비 존재
	if (MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Assign 실패 - %s 장비가 이미 등록됨"), *GetNameSafe(MeleeWeaponInstance))
		return false;
	}

	// 잘못된 아이템 장착 시도
	MeleeWeaponInstance = Cast<UMeleeWeaponInstance>(EquippableItemInstance);
	if (!MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Assign 실패 - %s: UMeleeWeaponInstance가 아님"), *GetNameSafe(EquippableItemInstance))
		return false;
	}

	// 장비 메시 설정
	if (EquipMeshComponent)
	{
		if (USkeletalMesh* Mesh = MeleeWeaponInstance->GetEquippableItemData()->EquipMesh)
		{
			EquipMeshComponent->SetSkeletalMesh(Mesh);
		}
	}

	// 등록 성공
	return true;
}

UEquippableItemInstance* UMeleeWeaponHandlerComponent::Unassign()
{
	// 등록된 장비 없음
	if (!MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Unassign 실패 - 등록된 장비가 없음"))
		return nullptr;
	}

	// 장착 해제 시도
	if (!Unequip())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Unassign 실패 - Unequip 실패"))
		return nullptr;
	}

	// 장비 메시 제거
	if (EquipMeshComponent)
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

	// 장비 제거
	UEquippableItemInstance* OutEquippableItemInstance = MeleeWeaponInstance;
	MeleeWeaponInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UMeleeWeaponHandlerComponent::Equip()
{
	// 등록된 장비 없음
	if (!MeleeWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Equip 실패 - 등록된 장비가 없음"))
		return false;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = MeleeWeaponInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Equip 실패 - %s: 유효하지 않은 EquippableItemData"), *GetNameSafe(MeleeWeaponInstance))
		return false;
	}

	// 캐릭터 메시 확인
	ACharacter* Character = GetOwner<ACharacter>();
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Equip 실패 - Owner가 Character가 아님 (Owner=%s)"), *GetNameSafe(GetOwner()))
		return false;
	}

	USkeletalMeshComponent* CharacterMeshComponent = Character->GetMesh();
	if (!CharacterMeshComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMeleeWeaponHandlerComponent] Equip 실패 - Character가 SkeletalMeshComponent를 갖지 않음 (Character=%s)"), *GetNameSafe(Character))
		return false;
	}

	// 메시 설정
	EquipMeshComponent->SetSkeletalMesh(EquippableItemData->EquipMesh);

	// 장착 소켓에 메시 부착
	const FName EquipSocketName = EquippableItemData->EquipSocketName;
	if (CharacterMeshComponent->DoesSocketExist(EquipSocketName))
	{
		EquipMeshComponent->AttachToComponent( // 소켓에 부착
			CharacterMeshComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			EquipSocketName);
	}

	return true;
}

bool UMeleeWeaponHandlerComponent::Unequip()
{
	return false;
}

bool UMeleeWeaponHandlerComponent::Use()
{
	return false;
}

bool UMeleeWeaponHandlerComponent::CanUnequip()
{
	return false;
}
