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
	UImage* CharacterPreviewImage;

	UFUNCTION()
	void OnNearbyItemsChanged(const TArray<AItemPickupBase*>& NearbyItems);

};
