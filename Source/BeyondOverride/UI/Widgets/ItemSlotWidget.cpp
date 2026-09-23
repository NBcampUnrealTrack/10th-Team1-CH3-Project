#include "UI/Widgets/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataTables/Items/ItemDataRow.h"
#include "DataTables/Items/RangeWeaponDataRow.h"
#include "Items/Objects/ItemInstanceBase.h"
#include "Items/Objects/RangeWeaponInstance.h"

void UItemSlotWidget::SetItem(UItemInstanceBase* Item, bool bUseLongImg)
{
	SlotData = Item;

	if (Item && Item->GetItemData())
	{
		if (IconImage)
		{
			UTexture2D* Icon = Item->GetItemData()->ItemIcon;

			if (bUseLongImg)
			{
				if (URangeWeaponInstance* RangeWeapon = Cast<URangeWeaponInstance>(Item))
				{
					if (const FRangeWeaponDataRow* RangeData = RangeWeapon->GetRangeWeaponData())
					{
						if (RangeData->ItemIconLong)
						{
							Icon = RangeData->ItemIconLong;
						}
					}
				}
			}

			IconImage->SetBrushFromTexture(Icon);
			IconImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (CountText)
		{
			if (Item->GetStackCount() > 1)
			{
				CountText->SetText(FText::AsNumber(Item->GetStackCount()));
				CountText->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				CountText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	else
	{
		if (IconImage)
		{
			IconImage->SetVisibility(ESlateVisibility::Hidden);
		}
		if (CountText)
		{
			CountText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClicked.Broadcast(SlotIndex, true);
		return FReply::Handled();
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnSlotClicked.Broadcast(SlotIndex, false);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlotHovered.Broadcast(true, SlotData);
}

void UItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	OnSlotHovered.Broadcast(false, nullptr);
}

