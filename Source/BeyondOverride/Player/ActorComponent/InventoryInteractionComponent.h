#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInteractionComponent.generated.h"

class UInventoryComponent;
class UPlayerInventoryComponent;
class UItemInstanceBase;

enum class EEquipmentSlot : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoldItemChanged, const UItemInstanceBase*, HoldItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInventoryInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// UI의 슬롯 클릭 처리
	UFUNCTION(BlueprintCallable)
	bool HandleSlotClick(UInventoryComponent* Inventory, int32 SlotIndex, bool bLeftClick); // 일반 칸 좌/우클릭
	UFUNCTION(BlueprintCallable)
	bool HandleEquipmentSlotClick(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot, bool bLeftClick); // 무기 칸 좌/우클릭
	UFUNCTION(BlueprintCallable)
	bool DropItem(bool bLeftClick); // 손에 들고 있는 아이템 버리기

	// 현재 손에 들고 있는 아이템 정보
	UFUNCTION(BlueprintPure)
	bool IsHoldingItem() const;
	UFUNCTION(BlueprintPure)
	UItemInstanceBase* GetHoldItem() const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHoldItemChanged OnHoldItemChanged;

public:
	UInventoryInteractionComponent();

private:
	// --------------- 일반 슬롯 함수 -------------------
	// 아이템 집기
	bool PickupAll(UInventoryComponent* Inventory, int32 SlotIndex);
	bool PickupHalf(UInventoryComponent* Inventory, int32 SlotIndex);

	// 아이템 놓기
	bool PlaceAll(UInventoryComponent* Inventory, int32 SlotIndex);
	bool PlaceOne(UInventoryComponent* Inventory, int32 SlotIndex);

	// 아이템 합치기
	bool MergeAll(UInventoryComponent* Inventory, int32 SlotIndex);
	bool MergeOne(UInventoryComponent* Inventory, int32 SlotIndex);

	// 아이템 교체
	bool SwapHeldItem(UInventoryComponent* Inventory, int32 SlotIndex);
	// --------------------------------------------------

	// --------------- 장비 슬롯 함수 -------------------
	// 아이템 집기
	bool PickupEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);
	bool PickupEquipmentHalf(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);

	// 아이템 놓기
	bool PlaceEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);
	bool PlaceEquipmentOne(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);

	// 아이템 합치기
	bool MergeEquipmentAll(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);
	bool MergeEquipmentOne(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);

	// 아이템 교체
	bool SwapEquipmentItem(UPlayerInventoryComponent* Inventory, EEquipmentSlot Slot);
	// --------------------------------------------------

	// ------------------ 공용 함수 ---------------------
	// 아이템 비교
	bool IsSameItem(const UItemInstanceBase* FirstItem, const UItemInstanceBase* SecondItem) const;

	// 아이템 복제
	UItemInstanceBase* CreateItemInstance(UItemInstanceBase* ItemInstance);

	// 아이템 버리기
	bool DropAll();
	bool DropOne();
	// --------------------------------------------------

private:
	UPROPERTY()
	TObjectPtr<UItemInstanceBase> HoldItem = nullptr;
};
