#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<UItemInstanceBase*>&, Slots);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 기본적인 인벤토리 조작
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemInstanceBase* Item, int32 SlotIndex = -1);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 SlotIndex, int32 Count);
	UFUNCTION(BlueprintCallable)
	bool SwapSlots(int32 FirstIndex, int32 SecondIndex);

	// 슬롯 접근
	UFUNCTION(BlueprintPure)
	TArray<UItemInstanceBase*> GetSlots() const;
	UFUNCTION(BlueprintPure)
	UItemInstanceBase* GetItem(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	bool SetSlots(const TArray<UItemInstanceBase*>& NewSlots);
	UFUNCTION(BlueprintCallable)
	bool SetItem(int32 SlotIndex, UItemInstanceBase* Item);

	// 아이템 수량 변경
	UFUNCTION(BlueprintCallable)
	bool SetItemStackCount(int32 SlotIndex, int32 StackCount);

	// 슬롯 정보
	UFUNCTION(BlueprintPure)
	int32 GetSlotCount() const;
	UFUNCTION(BlueprintPure)
	bool IsValidSlot(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	bool FindEmptySlotIndex(int32& EmptySlotIndex) const;

	int32 FindItemIndex(const FName& ItemID) const;
	int32 GetItemCount(const FName& ItemID) const;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void NotifyInventoryChanged();

private:
	void InitializeSlot();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemInstanceBase>> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxSlotCount = 16;
};
