#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "EquipmentSlotWidget.generated.h"

class UPlayerInventoryComponent;
class UInventoryInteractionComponent;
class UEquipmentManagerComponent;
class UEquippableItemInstance;
class UItemSlotWidget;
class UItemInstanceBase;
class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, Category = "Equipment")
	EEquipmentSlot EquipmentSlot;

	void SetupEquipmentSlot(
		UPlayerInventoryComponent* InInventory,
		UInventoryInteractionComponent* InInteraction,
		UEquipmentManagerComponent* InEquipmentManager);

	UFUNCTION(BlueprintCallable, Category = "Rarity")
	void SetShowRarity(bool bInShowRarity);

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotWidget> ItemSlot;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> WpnName;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> WpnNumberInHead;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> WpnNumberInBody;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> CurrentAmmoCount;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> CurrentAmmoCountInBody;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> TotalAmmoCount;
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> TotalAmmoCountInBody;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> WpnAmmoType;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UWidget> WBP_Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	bool bShowRarity = true;

  private:
	TMap<int32, FString> SlotEmptyNameMap = {
		{1, TEXT("주무기")},
		{2, TEXT("보조무기")},
		{3, TEXT("근접무기")},
		{4, TEXT("투척")},
		{5, TEXT("회복")},
		{6, TEXT("가방")},
		{7, TEXT("쉴드")},
	};

	UPROPERTY()
	TObjectPtr<UPlayerInventoryComponent> InventoryComponent;
	UPROPERTY()
	TObjectPtr<UInventoryInteractionComponent> InteractionComponent;
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	UFUNCTION()
	void OnEquipmentSlotChanged(EEquipmentSlot ChangedSlot, UItemInstanceBase* ItemInstanceBase);

	// 인벤토리 슬롯 내용/수량 변경 시 호출 (총 탄약 개수 갱신용)
	UFUNCTION()
	void OnInventoryChanged(const TArray<UItemInstanceBase*>& Slots);

	void OnActiveSlotChanged(EEquipmentSlot ChangedSlot, UEquippableItemInstance* ItemInstance);

	void OnRangeWeaponAmmoCountUpdated(EEquipmentSlot FiredSlot, int32 AmmoCount);

	UFUNCTION()
	void HandleItemSlotClicked(int32 SlotIndex, bool bLeftClick);

	void RefreshItem();
	void RefreshEquippedBadge();
};
