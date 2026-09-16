#include "ActorComponents/EquipmentHandlerComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "GameFramework/Character.h"
#include "Items/Objects/EquippableItemInstance.h"

UEquipmentHandlerComponent::UEquipmentHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EquipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Equipment Mesh"));
}

void UEquipmentHandlerComponent::OnRegister()
{
	Super::OnRegister();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		EquipMeshComponent->SetupAttachment(Character->GetMesh());
	}
}

bool UEquipmentHandlerComponent::HasEquipment() const
{
	return EquippableItemInstance != nullptr;
}

UEquippableItemInstance* UEquipmentHandlerComponent::GetEquippableItemInstance() const
{
	return EquippableItemInstance;
}

bool UEquipmentHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 장비 인스턴스 저장
	EquippableItemInstance = InEquippableItemInstance;

	return true;
}

UEquippableItemInstance* UEquipmentHandlerComponent::Unassign()
{
	if (!CanUnassign())
	{
		return nullptr;
	}

	// 장비 메시 제거
	if (EquipMeshComponent)
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

	// 장비 데이터 제거
	UEquippableItemInstance* OutEquippableItemInstance = EquippableItemInstance;
	EquippableItemInstance = nullptr;

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UEquipmentHandlerComponent::Equip()
{
	if (!CanEquip())
	{
		return false;
	}

	// 메시 설정
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData(); // 장비 데이터
	EquipMeshComponent->SetSkeletalMesh(EquippableItemData->EquipMesh);                                 // 장비 메시 설정

	// 장착 소켓에 메시 부착
	AttachToSocket(EquippableItemData->EquipSocketName);

	return true;
}

bool UEquipmentHandlerComponent::Unequip()
{
	if (!CanUnequip())
	{
		return false;
	}

	// 보관 소켓에 메시 부착 - 없으면 숨기기
	AttachToSocket(EquippableItemData->HolsterSocketName, true);

	return true;
}

bool UEquipmentHandlerComponent::Use()
{
	return true;
}

void UEquipmentHandlerComponent::StartAction()
{
}

void UEquipmentHandlerComponent::EndAction()
{
}

bool UEquipmentHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	// 이미 등록된 장비 데이터 존재
	if (EquippableItemInstance)
	{
		return false;
	}

	// 유효하지 않은 EquippableItemInstance
	if (!InEquippableItemInstance)
	{
		return false;
	}

	// 유효하지 않은 EquippableItemData
	if (!InEquippableItemInstance->GetEquippableItemData())
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUnassign() const
{
	// 등록된 장비 없음
	if (!EquippableItemInstance)
	{
		return false;
	}

	// 장비 해제 불가
	if (!CanUnequip())
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanEquip() const
{
	// 등록된 장비 없음
	if (!EquippableItemInstance)
	{
		return false;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		return false;
	}

	// 캐릭터 메시 확인
	ACharacter* Character = GetOwner<ACharacter>();
	if (!Character)
	{
		return false;
	}

	USkeletalMeshComponent* CharacterMeshComponent = Character->GetMesh();
	if (!CharacterMeshComponent)
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUnequip() const
{
	// 등록된 장비 없음
	if (!EquippableItemInstance)
	{
		return false;
	}

	// 데이터 유효성 검증
	const FEquippableItemDataRow* EquippableItemData = EquippableItemInstance->GetEquippableItemData();
	if (!EquippableItemData)
	{
		return false;
	}

	// 캐릭터 메시 확인
	ACharacter* Character = GetOwner<ACharacter>();
	if (!Character)
	{
		return false;
	}

	USkeletalMeshComponent* CharacterMeshComponent = Character->GetMesh();
	if (!CharacterMeshComponent)
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUse() const
{
	// 등록된 장비 없음
	if (!EquippableItemInstance)
	{
		return false;
	}

	return true;
}

void UEquipmentHandlerComponent::AttachToSocket(const FName& SocketName, bool bHideIfNoSocket)
{
	// 장비 메시 컴포넌트가 유효하지 않음
	if (!EquipMeshComponent)
	{
		return;
	}

	// 장착 소켓에 메시 부착
	USkeletalMeshComponent* CharacterMeshComponent = GetOwner<ACharacter>()->GetMesh(); // 캐릭터 메시
	if (CharacterMeshComponent->DoesSocketExist(SocketName))                            // 소켓 존재
	{
		EquipMeshComponent->AttachToComponent( // 소켓에 부착
			CharacterMeshComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName);
	}
	else if (bHideIfNoSocket) // 소켓 없음 & bHideIfNoSocket == true -> 메시 제거
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}
}
