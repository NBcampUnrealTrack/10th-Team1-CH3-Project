#pragma once

#include "CoreMinimal.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

class UItemInstanceBase;

enum class EEquipmentSlot : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnEquipmentSlotChanged,
	EEquipmentSlot, Slot,
	UItemInstanceBase*, ItemInstanceBase
);

UCLASS()
class BEYONDOVERRIDE_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
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

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotChanged OnEquipmentSlotChanged;

public:
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeEquipmentSlot();
	int32 GetEquipmentSlotIndex(EEquipmentSlot Slot) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemInstanceBase>> EquipmentSlots;
};
