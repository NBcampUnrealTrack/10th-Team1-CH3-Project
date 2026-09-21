#pragma once

#include "CoreMinimal.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

struct FBackpackDataRow;

class UItemInstanceBase;

enum class EEquipmentSlot : uint8;

// UI용 아이템 개수 변경 시
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnEquipmentSlotChanged,
	EEquipmentSlot, Slot,
	UItemInstanceBase*, ItemInstanceBase
);

// assign/unassign용 아이템 자체가 변경 시
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnEquipmentItemChanged,
	EEquipmentSlot, Slot,
	UItemInstanceBase*, ItemInstanceBase
);

// 무게 변경
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeightChanged,
	float, CurCarryWeight,
	float, MaxCarryWeight
);

// 돈 변경
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnMoneyChanged,
	int32, Money
);

UCLASS()
class BEYONDOVERRIDE_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	// Slots getter/setter
	UFUNCTION(BlueprintPure)
	TArray<UItemInstanceBase*> GetEquipmentSlots() const { return EquipmentSlots; }

	UFUNCTION(BlueprintCallable)
	bool SetEquipmentSlots(const TArray<UItemInstanceBase*>& NewSlots);

	// 장비 슬롯 조작
	UFUNCTION(BlueprintCallable)
	bool SetEquipmentItem(EEquipmentSlot Slot, UItemInstanceBase* Item);

	UFUNCTION(BlueprintPure)
	UItemInstanceBase* GetEquipmentItem(EEquipmentSlot Slot) const;

	UFUNCTION(BlueprintPure)
	bool IsValidEquipmentSlot(EEquipmentSlot Slot) const;

	UFUNCTION(BlueprintPure)
	bool CanEquipItem(EEquipmentSlot Slot, const UItemInstanceBase* Item) const;

	// 장비 슬롯 아이템 수량 변경
	UFUNCTION(BlueprintCallable)
	bool SetEquipmentItemStackCount(EEquipmentSlot Slot, int32 StackCount);

	UFUNCTION(BlueprintPure)
	float GetCurCarryWeight() const { return CurCarryWeight; }

	UFUNCTION(BlueprintPure)
	float GetMaxCarryWeight() const { return MaxCarryWeight; }

	// 가방을 적용했을 때, 슬롯 부족으로 들어가지 못한 아이템 반환
	TArray<UItemInstanceBase*> ApplyBackpack(const FBackpackDataRow* BackpackData);


	// 일반 슬롯 + 장비 슬롯의 총 아이템 개수 반환
	UFUNCTION(BlueprintPure)
	int32 GetTotalItemCount(FName ItemID) const;

	// ItemID를 Count만큼 판매
	UFUNCTION(BlueprintCallable)
	bool SellItem(FName ItemID, int32 Count);

	// 돈 사용
	UFUNCTION(BlueprintCallable)
	bool SpendMoney(int32 Amount);

	// 현재 보유 돈 반환
	UFUNCTION(BlueprintPure)
	int32 GetMoney() const { return Money; }

	// 돈 설정
	void SetMoney(int32 NewMoney);

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotChanged OnEquipmentSlotChanged;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentItemChanged OnEquipmentItemChanged;
	UPROPERTY(BlueprintAssignable)
	FOnWeightChanged OnWeightChanged;
	UPROPERTY(BlueprintAssignable)
	FOnMoneyChanged OnMoneyChanged;

public:
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void NotifyInventoryChanged() override;
	void RecalculateCarryWeight();

private:
	void InitializeEquipmentSlot();
	int32 GetEquipmentSlotIndex(EEquipmentSlot Slot) const;
	EEquipmentSlot GetEquipmentSlotType(int32 SlotIndex) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemInstanceBase>> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float CurCarryWeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float MaxCarryWeight = 50.0f;

	int32 BaseSlotCount = 0;
	float BaseMaxCarryWeight = 0.f;

	int32 Money = 0;
};
