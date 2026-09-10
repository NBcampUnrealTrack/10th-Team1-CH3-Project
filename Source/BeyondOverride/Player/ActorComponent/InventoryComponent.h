#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<UItemInstanceBase*>&, Slots);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeightChanged, float, CurCarryWeight, float, MaxCarryWeight);

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
	UFUNCTION(BlueprintCallable)
	UItemInstanceBase* GetItem(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	bool SetItem(int32 SlotIndex, UItemInstanceBase* Item);

	// 슬롯 정보
	UFUNCTION(BlueprintCallable)
	int32 GetSlotCount() const;
	UFUNCTION(BlueprintCallable)
	bool IsValidSlot(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable)
	bool FindEmptySlotIndex(int32& EmptySlotIndex) const;

	UFUNCTION(BlueprintCallable)
	void NotifyInventoryChanged();

public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
	UPROPERTY(BlueprintAssignable)
	FOnWeightChanged OnWeightChanged;

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeSlot();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UItemInstanceBase>> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxSlotCount = 16;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float CurCarryWeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float MaxCarryWeight = 500.0f;
};
