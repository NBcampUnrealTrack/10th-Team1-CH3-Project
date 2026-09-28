#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ItemTooltipWidget.generated.h"

class UItemInstanceBase;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class BEYONDOVERRIDE_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnItemHovered(bool bIsHovered, UItemInstanceBase* Item, bool bInShowBuyPrice = false);

	void SetTooltipData();

  private:
	TObjectPtr<UItemInstanceBase> SlotData;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Price;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemTotalPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemUnitPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemTotalWeightText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemUnitWeightText;

	bool bShowBuyPrice = false;

	void UpdatePosition();
};
