#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ItemSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UItemInstanceBase;
class UItemRarityStyleDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotClicked, int32, SlotIndex, bool, bLeftClick);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotHovered, bool, bIsHovered, UItemInstanceBase*, SlotData);

UCLASS()
class BEYONDOVERRIDE_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void SetSlotIndex(int32 InIndex)
	{
		SlotIndex = InIndex;
	}
	void SetItem(UItemInstanceBase* Item, bool bUseLongImg = false);

	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;

	UPROPERTY(BlueprintAssignable)
	FOnSlotHovered OnSlotHovered;

	UFUNCTION(BlueprintCallable, Category = "Rarity")
	void SetShowRarity(bool bInShowRarity);

  protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemRarity;

	UPROPERTY(EditDefaultsOnly, Category = "Rarity")
	TObjectPtr<UItemRarityStyleDataAsset> RarityStyleData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	bool bShowRarity = true;

  private:
	int32 SlotIndex = -1;
	TObjectPtr<UItemInstanceBase> SlotData;
};
