#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ShopScreenWidget.generated.h"

class UItemSlotPanelWidget;
class UItemTooltipWidget;
class UHeldItemWidget;
class UItemInstanceBase;
class UTextBlock;
class UPlayerInventoryComponent;

UCLASS()
class BEYONDOVERRIDE_API UShopScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void SetShopItems(const TArray<UItemInstanceBase*>& InItems);

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotPanelWidget> ShopSlotPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotPanelWidget> BackpackSlotPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemTooltipWidget> Tooltip;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHeldItemWidget> HeldItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Money;

  private:
	UFUNCTION()
	void HandleSlotHovered(bool bIsHovered, UItemInstanceBase* Item, bool bIsBuy);

	UPROPERTY()
	TArray<TObjectPtr<UItemInstanceBase>> CachedShopItems;
	UPROPERTY()

	TObjectPtr<UPlayerInventoryComponent> Inventory;

	UFUNCTION()
	void HandleMoneyChanged(int32 Money);

	UFUNCTION()
	void HandleHoldItemChanged(const UItemInstanceBase* HoldItem);
};
