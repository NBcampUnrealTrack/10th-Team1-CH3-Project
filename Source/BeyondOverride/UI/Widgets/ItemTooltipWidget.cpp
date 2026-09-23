#include "UI/Widgets/ItemTooltipWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Items/Objects/ItemInstanceBase.h"

void UItemTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UItemTooltipWidget::OnItemHovered(bool bIsHovered, UItemInstanceBase* Item)
{
	SlotData = Item;
	SetVisibility(bIsHovered && Item ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	if (bIsHovered && SlotData)
	{
		SetTooltipData();
		UpdatePosition();	
	}
}

void UItemTooltipWidget::SetTooltipData()
{
	if (!SlotData)
		return;

	if (const FItemDataRow* Data = SlotData->GetItemData())
	{
		ItemNameText->SetText(Data->DisplayName);
		ItemDescText->SetText(Data->Description);
		ItemPriceText->SetText(FText::AsNumber(Data->SellPrice));
		ItemWeightText->SetText(FText::AsNumber(Data->Weight));
	}
}

void UItemTooltipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Collapsed)
		return;

	UpdatePosition();
}

void UItemTooltipWidget::UpdatePosition()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
		return;

	float MouseX = 0.f, MouseY = 0.f;

	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PC, MouseX, MouseY);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		CanvasSlot->SetPosition(FVector2D(MouseX + 35.f, MouseY + 35.f));
	}
}
