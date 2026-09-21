#include "ActorComponents/EquipmentHandlerComponent.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataAssets/EquipmentAnimationDataAsset.h"
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

	// 장비 데이터 저장
	ItemData = InEquippableItemInstance->GetItemData();
	EquippableItemData = InEquippableItemInstance->GetEquippableItemData();

	return true;
}

UEquippableItemInstance* UEquipmentHandlerComponent::Unassign()
{
	if (!CanUnassign())
	{
		return nullptr;
	}

	if (IsEquipping())
	{
		OnEquipInterrupted();
	}

	// 장비 메시 제거
	if (EquipMeshComponent)
	{
		EquipMeshComponent->SetSkeletalMesh(nullptr);
	}

	// 장비 제거
	UEquippableItemInstance* OutEquippableItemInstance = EquippableItemInstance;
	EquippableItemInstance = nullptr;

	// 장비 데이터 제거
	ItemData = nullptr;
	EquippableItemData = nullptr;

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
	EquipMeshComponent->SetSkeletalMesh(EquippableItemData->EquipMesh); // 장비 메시 설정

	// 장착 소켓에 메시 부착
	AttachToSocket(EquippableItemData->EquipSocketName);

	// 장착 딜레이 시작
	OnEquipStarted();

	return true;
}

bool UEquipmentHandlerComponent::Unequip()
{
	if (!CanUnequip())
	{
		return false;
	}

	if (IsEquipping())
	{
		OnEquipInterrupted();
	}

	// 보관 소켓에 메시 부착 - 없으면 숨기기
	AttachToSocket(EquippableItemData->HolsterSocketName, true);

	return true;
}

bool UEquipmentHandlerComponent::Use()
{
	return CanUse();
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
	if (HasEquipment())
	{
		return false;
	}

	// 유효하지 않은 EquippableItemInstance
	if (!InEquippableItemInstance)
	{
		return false;
	}

	// 유효하지 않은 ItemData
	if (!InEquippableItemInstance->GetItemData())
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
	if (!HasEquipment())
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
	if (!HasEquipment())
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUnequip() const
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return false;
	}

	if (IsEquipping())
	{
		return false;
	}

	return true;
}

bool UEquipmentHandlerComponent::CanUse() const
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return false;
	}

	// 장착 중인 경우
	if (IsEquipping())
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

void UEquipmentHandlerComponent::OnEquipStarted()
{
	//// 장착 시작 디버그 메시지 출력
	//GEngine->AddOnScreenDebugMessage(1100, 5.0f, FColor::Silver, FString::Printf(TEXT("Equip Started - %s"), *GetNameSafe(EquippableItemInstance)));

	//// 장비 장착 타이머 활성화
	//if (UWorld* World = GetWorld())
	//{
	//	GetWorld()->GetTimerManager().SetTimer(
	//		EquipTimerHandle,
	//		this,
	//		&UEquipmentHandlerComponent::OnEquipCompleted,
	//		EquippableItemData->EquipDelay,
	//		false);
	//}

	GEngine->AddOnScreenDebugMessage(1100, 5.0f, FColor::Silver, FString::Printf(TEXT("Equip Started - %s"), *GetNameSafe(EquippableItemInstance)));

	if (!EquippableItemData)
	{
		return;
	}

	UEquipmentAnimationDataAsset* AnimationData = EquippableItemData->EquipmentAnimationData;
	UAnimMontage* EquipMontage = AnimationData ? AnimationData->Equip : nullptr;

	// Equip 몽타주가 없다면 즉시 장착 완료
	if (!EquipMontage)
	{
		OnEquipCompleted();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = Character ? Character->GetMesh() : nullptr;

	UAnimInstance* AnimInstance = CharacterMesh ? CharacterMesh->GetAnimInstance() : nullptr;

	// AnimInstance가 없으면 몽타주를 재생할 수 없으므로 즉시 완료
	if (!AnimInstance)
	{
		OnEquipCompleted();
		return;
	}

	bIsEquipping = true;
	PlayingEquipMontage = EquipMontage;

	const float PlayDuration = AnimInstance->Montage_Play(EquipMontage);

	// 몽타주 재생 실패
	if (PlayDuration <= 0.f)
	{
		bIsEquipping = false;
		PlayingEquipMontage = nullptr;

		OnEquipCompleted();
		return;
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UEquipmentHandlerComponent::HandleEquipMontageEnded);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, EquipMontage);
}

void UEquipmentHandlerComponent::OnEquipCompleted()
{
	bIsEquipping = false;
	PlayingEquipMontage = nullptr;

	// 장착 종료 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1100, 5.0f, FColor::Silver, FString::Printf(TEXT("Equip Completed - %s"), *GetNameSafe(EquippableItemInstance)));

	//// 장비 장착 타이머 명시적으로 제거
	//if (UWorld* World = GetWorld())
	//{
	//	World->GetTimerManager().ClearTimer(EquipTimerHandle);
	//}
}

void UEquipmentHandlerComponent::OnEquipInterrupted()
{
	// 장착 중단 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(1100, 5.0f, FColor::Silver, FString::Printf(TEXT("Equip Interrupted - %s"), *GetNameSafe(EquippableItemInstance)));

	//// 장비 장착 타이머 제거
	//if (UWorld* World = GetWorld())
	//{
	//	World->GetTimerManager().ClearTimer(EquipTimerHandle);
	//}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* CharacterMesh = Character ? Character->GetMesh() : nullptr;
	UAnimInstance* AnimInstance = CharacterMesh ? CharacterMesh->GetAnimInstance() : nullptr;
	UAnimMontage* MontageToStop = PlayingEquipMontage;

	bIsEquipping = false;
	PlayingEquipMontage = nullptr;

	if (AnimInstance && MontageToStop && AnimInstance->Montage_IsPlaying(MontageToStop))
	{
		AnimInstance->Montage_Stop(0.1f, MontageToStop);
	}
}

bool UEquipmentHandlerComponent::IsEquipping() const
{
	/*const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	return World->GetTimerManager().IsTimerActive(EquipTimerHandle);*/

	return bIsEquipping;
}

void UEquipmentHandlerComponent::HandleEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != PlayingEquipMontage)
	{
		return;
	}

	PlayingEquipMontage = nullptr;
	bIsEquipping = false;

	if (bInterrupted)
	{
		OnEquipInterrupted();
		return;
	}

	OnEquipCompleted();
}
