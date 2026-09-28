#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "InventoryScreenWidget.generated.h"

class UItemSlotPanelWidget;
class UInventoryComponent;
class UHeldItemWidget;
class AItemPickupBase;
class UNearbyItemComponent;
class UVerticalBox;
class UImage;
class UItemTooltipWidget;
class UItemInstanceBase;

UCLASS()
class BEYONDOVERRIDE_API UInventoryScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

  public:
	void OpenContainer(UInventoryComponent* ContainerInventory, const FText& ContainerName);
	void CloseContainer();

  protected:
	UPROPERTY(meta = (BindWidget))
	UItemSlotPanelWidget* ContainerSlotPanel;

	UPROPERTY(meta = (BindWidget))
	UItemSlotPanelWidget* BackpackSlotPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> EquipmentSlotArea;

	UPROPERTY(meta = (BindWidget))
	UHeldItemWidget* HeldItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemTooltipWidget> Tooltip;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPreviewImage;

	UFUNCTION()
	void OnNearbyItemsChanged(const TArray<AItemPickupBase*>& NearbyItems);

	UFUNCTION()
	void HandleSlotHovered(bool bIsHovered, UItemInstanceBase* Item, bool bIsBuy);

	UFUNCTION()
	void HandleHoldItemChanged(const UItemInstanceBase* HoldItem);

  private:
	bool bIsContainerOpen = false;
};
