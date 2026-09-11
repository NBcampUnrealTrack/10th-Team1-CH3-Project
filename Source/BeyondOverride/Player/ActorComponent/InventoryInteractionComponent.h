#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInteractionComponent.generated.h"

class UInventoryComponent;
class UItemInstanceBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoldItemChanged, const UItemInstanceBase*, HoldItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInventoryInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// UI의 슬롯 클릭 처리
	UFUNCTION(BlueprintCallable)
	bool HandleSlotClick(UInventoryComponent* Inventory, int32 SlotIndex, bool bLeftClick);

	// 현재 들고 있는 아이템
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

	// 아이템 비교
	bool IsSameItem(const UItemInstanceBase* FirstItem, const UItemInstanceBase* SecondItem) const;

	// 아이템 복제
	UItemInstanceBase* CreateItemInstance(UItemInstanceBase* ItemInstance);

private:
	UPROPERTY()
	TObjectPtr<UItemInstanceBase> HoldItem = nullptr;
};
