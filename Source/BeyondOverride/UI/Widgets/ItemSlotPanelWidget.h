#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ItemSlotPanelWidget.generated.h"

class UInventoryComponent;
class UInventoryInteractionComponent;
class UUniformGridPanel;
class UItemSlotWidget;
class UItemInstanceBase;
class AItemPickupBase;
class UPanelFrameWidget;
class UNearbyItemComponent;
class UPlayerInventoryComponent;

UENUM()
enum class EItemSlotPanelMode : uint8
{
	Inventory,
	WorldItems,
};

UCLASS()
class BEYONDOVERRIDE_API UItemSlotPanelWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

  public:
	UItemSlotPanelWidget(const FObjectInitializer& ObjectInitializer);

	void SetInventory(UInventoryComponent* InInventory, UInventoryInteractionComponent* InInteraction);

	void SetWorldItems(const TArray<AItemPickupBase*>& InItems, UNearbyItemComponent* InNearbyItemComponent, UInventoryInteractionComponent* InInteraction);

	void RefreshSlots();

	void SetContainerName(const FText& InName);

  protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* SlotContainer;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UPanelFrameWidget> PanelFrame;

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	TSubclassOf<UItemSlotWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Slot")
	int32 ColumnCount = 5;

  private:
	EItemSlotPanelMode Mode = EItemSlotPanelMode::WorldItems;

	UPROPERTY()
	TObjectPtr<UNearbyItemComponent> NearbyItemComponent;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TArray<TObjectPtr<AItemPickupBase>> WorldItems;

	UPROPERTY()
	TObjectPtr<UInventoryInteractionComponent> InteractionComponent;

	UPROPERTY()
	TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent;

	void UnbindInventory();

	UFUNCTION()
	void OnInventoryChanged(const TArray<UItemInstanceBase*>& Slots);

	UFUNCTION()
	void HandleSlotClicked(int32 SlotIndex, bool bLeftClick);

	UFUNCTION()
	void OnWeightChanged(float CurCarryWeight, float MaxCarryWeight);
};
