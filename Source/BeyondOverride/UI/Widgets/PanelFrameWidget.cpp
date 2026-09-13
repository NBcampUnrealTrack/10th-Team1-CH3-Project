#include "UI/Widgets/PanelFrameWidget.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

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
