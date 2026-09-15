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
	TObjectPtr<UTextBlock> CurrentAmmoCount;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> TotalAmmoCount;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> WpnAmmoType;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UWidget> WBP_Equipped;

  private:
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
