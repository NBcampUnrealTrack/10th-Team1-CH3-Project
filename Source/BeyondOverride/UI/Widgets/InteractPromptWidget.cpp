#include "UI/Widgets/InteractPromptWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInteractPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	if (HoldBar)
	{
		HoldBar->SetPercent(0.f);
		HoldBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractPromptWidget::HandleFocusChanged(bool bHasTarget,
											 FInteractPrompt Data)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleFocusChanged called: bHasTarget=%s, Title=%s"), bHasTarget ? TEXT("true") : TEXT("false"), *Data.Title.ToString());

	if (!bHasTarget)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);

	if (TitleText)
	{
		TitleText->SetText(Data.Title);
	}

	if (ActionText)
	{
		ActionText->SetText(Data.bEnabled ? Data.ActionText
										  : Data.DisableReason);
		ActionText->SetColorAndOpacity(
			Data.bEnabled ? FSlateColor(FLinearColor::White)
						  : FSlateColor(FLinearColor(1.f, 0.35f, 0.35f)));
	}
}

void UInteractPromptWidget::HandleHoldProgress(float Progress)
{
	if (!HoldBar)
		return;

	HoldBar->SetPercent(Progress);
	HoldBar->SetVisibility(Progress > 0.f ? ESlateVisibility::HitTestInvisible
										  : ESlateVisibility::Collapsed);
}
