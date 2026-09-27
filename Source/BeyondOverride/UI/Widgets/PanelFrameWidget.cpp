#include "UI/Widgets/PanelFrameWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UPanelFrameWidget::SetContainerName(const FText& InName)
{
	if (ContainerNameText)
		ContainerNameText->SetText(InName);
}

void UPanelFrameWidget::SetSlotCount(int32 CurrentCount, int32 MaxCount)
{
	CountTextBox->SetVisibility(ESlateVisibility::Visible);

	if (CurrentCountText)
		CurrentCountText->SetText(FText::AsNumber(CurrentCount));

	if (MaxCountText)
		MaxCountText->SetText(FText::AsNumber(MaxCount));
}

void UPanelFrameWidget::SetCarryWeight(float CurrentWeight, float MaxWeight)
{
	if (CarryWeightBox)
		CarryWeightBox->SetVisibility(ESlateVisibility::Visible);

	if (CurrentCarryWeightText)
		CurrentCarryWeightText->SetText(FText::AsNumber(CurrentWeight));

	if (MaxCarryWeightText)
		MaxCarryWeightText->SetText(FText::AsNumber(MaxWeight));

	bIsOverCarryWeight = (CurrentWeight > MaxWeight);
}

void UPanelFrameWidget::HideCountTextBox()
{
	if (CountTextBox)
		CountTextBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UPanelFrameWidget::HideCarryWeight()
{
	if (CarryWeightBox)
		CarryWeightBox->SetVisibility(ESlateVisibility::Collapsed);
}
