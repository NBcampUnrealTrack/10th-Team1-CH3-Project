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

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

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

	void OnActiveSlotChanged(EEquipmentSlot ChangedSlot, UEquippableItemInstance* ItemInstance);

	UFUNCTION()
	void HandleItemSlotClicked(int32 SlotIndex, bool bLeftClick);

	void RefreshItem();
	void RefreshEquippedBadge();
};
