#include "UI/Widgets/ItemSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataTables/Items/ItemDataRow.h"
#include "Items/Objects/ItemInstanceBase.h"

void UItemSlotWidget::SetItem(UItemInstanceBase* Item, bool bUseLongImg)
{
	if (Item && Item->GetItemData())
	{
		if (IconImage)
		{
			IconImage->SetBrushFromTexture(bUseLongImg ? Item->GetItemData()->ItemIcon : Item->GetItemData()->ItemIcon);
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
