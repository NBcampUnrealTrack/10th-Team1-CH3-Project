#include "ActorComponents/UtilityItemHandlerComponent.h"

#include "DataTables/Items/EquippableItemDataRow.h"
#include "DataTables/Items/UtilityItemDataRow.h"
#include "Items/Objects/EquippableItemInstance.h"
#include "Items/Objects/UtilityItemInstance.h"

UUtilityItemHandlerComponent::UUtilityItemHandlerComponent()
{
	UtilityItemInstance = nullptr;
}

bool UUtilityItemHandlerComponent::Assign(UEquippableItemInstance* InEquippableItemInstance)
{
	if (!Super::Assign(InEquippableItemInstance))
	{
		return false;
	}

	// Utility Item 인스턴스 저장
	UtilityItemInstance = Cast<UUtilityItemInstance>(EquippableItemInstance);

	// Utility Item 데이터 저장
	UtilityItemData = UtilityItemInstance->GetUtilityItemData();

	// 유효하지 않은 데이터
	if (!UtilityItemData)
	{
		UtilityItemInstance = nullptr;
		return false;
	}

	// Assign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5000, 5.0f, FColor::Green, FString::Printf(TEXT("Utility Item Assigned - %s"), *GetNameSafe(EquippableItemInstance)));

	// 등록 성공
	return true;
}

UEquippableItemInstance* UUtilityItemHandlerComponent::Unassign()
{
	UEquippableItemInstance* OutEquippableItemInstance = Super::Unassign();
	if (!OutEquippableItemInstance)
	{
		return nullptr;
	}

	// Utility Item 인스턴스 제거
	UtilityItemInstance = nullptr;

	// Utility Item 데이터 제거
	UtilityItemData = nullptr;

	// Unassign 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5000, 5.0f, FColor::Green, FString::Printf(TEXT("Utility Item Unassigned - %s"), *GetNameSafe(EquippableItemInstance)));

	// 제거한 장비 반환
	return OutEquippableItemInstance;
}

bool UUtilityItemHandlerComponent::Equip()
{
	if (!Super::Equip())
	{
		return false;
	}

	// Equip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5001, 5.0f, FColor::Green, FString::Printf(TEXT("Utility Item Equipped - %s"), *GetNameSafe(EquippableItemInstance)));

	return true;
}

bool UUtilityItemHandlerComponent::Unequip()
{
	if (!Super::Unequip())
	{
		return false;
	}

	// 사용 중이면 취소
	OnUseInterrupted();

	// Unequip 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5001, 5.0f, FColor::Green, FString::Printf(TEXT("Utility Item Unequipped - %s"), *GetNameSafe(EquippableItemInstance)));

	return true;
}

bool UUtilityItemHandlerComponent::Use()
{
	if (!CanUse())
	{
		return false;
	}

	// 사용 시작
	OnUseStarted();

	return true;
}

void UUtilityItemHandlerComponent::StartAction()
{
}

void UUtilityItemHandlerComponent::EndAction()
{
}

bool UUtilityItemHandlerComponent::CanAssign(const UEquippableItemInstance* InEquippableItemInstance) const
{
	if (!Super::CanAssign(InEquippableItemInstance))
	{
		return false;
	}

	// 잘못된 아이템 타입
	if (!InEquippableItemInstance->IsA(UUtilityItemInstance::StaticClass()))
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::CanUnassign() const
{
	return Super::CanUnassign();
}

bool UUtilityItemHandlerComponent::CanEquip() const
{
	return Super::CanEquip();
}

bool UUtilityItemHandlerComponent::CanUnequip() const
{
	if (!Super::CanUnequip())
	{
		return false;
	}

	return true;
}

bool UUtilityItemHandlerComponent::CanUse() const
{
	if (!Super::CanUse())
	{
		return false;
	}

	// 사용 가능한지 델리게이트로 확인
	if (!CanUseUtilityItemDelegate.IsBound() ||
		!CanUseUtilityItemDelegate.Execute(UtilityItemData))
	{
		return false;
	}

	return true;
}

void UUtilityItemHandlerComponent::OnUseStarted()
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return;
	}

	// 사용 타이머 실행
	GetWorld()->GetTimerManager().SetTimer(
		UseTimerHandle,
		this,
		&UUtilityItemHandlerComponent::OnUseCompleted,
		UtilityItemData->UseDuration,
		false);

	// 사용 시작 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5002, 5.0f, FColor::Green, FString::Printf(TEXT("Using Utility Started")));
}

void UUtilityItemHandlerComponent::OnUseCompleted()
{
	// 등록된 장비 없음
	if (!HasEquipment())
	{
		return;
	}

	// 사용 후 개수 차감
	const int Count = UtilityItemInstance->GetStackCount();
	UtilityItemInstance->SetStackCount(Count - 1);

	// 사용 후 개수 변경 델리게이트 송출
	OnCountUpdatedDelegate.Broadcast(UtilityItemInstance);

	// 효과 적용 델리게이트 송출
	OnEffectAppliedDelegate.Broadcast(UtilityItemData);

	// 사용 완료 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5002, 5.0f, FColor::Green, FString::Printf(TEXT("Using Utility Completed")));
}

void UUtilityItemHandlerComponent::OnUseInterrupted()
{
	// 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(UseTimerHandle);

	// 사용 취소 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(5002, 5.0f, FColor::Green, FString::Printf(TEXT("Using Utility Interrupted")));
}
