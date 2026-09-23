#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ItemTooltipWidget.generated.h"

class UItemInstanceBase;
class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnItemHovered(bool bIsHovered, UItemInstanceBase* Item);

	void SetTooltipData();

  private:
	TObjectPtr<UItemInstanceBase> SlotData;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemWeightText;

	void UpdatePosition();
};
