#include "UI/Widgets/InteractPromptWidget.h"

#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"

void UInteractPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	if (ProgressFill)
	{
		// ProgressFill 의 브러시에 M_SqaureProgress 가 지정되어있어야함
		// GetDynamicMaterial()이 머티리얼의 복제본을 만들어 브러시에 자동으로 적용해줌
		ProgressMat = ProgressFill->GetDynamicMaterial();
		ProgressFill->SetVisibility(ESlateVisibility::Collapsed);
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

	if (PressProgressBarBox)
	{
		PressProgressBarBox->SetVisibility(Data.bEnabled ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}

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
	if (!ProgressMat || !ProgressFill)
		return;

	ProgressMat->SetScalarParameterValue(TEXT("Progress"), Progress);
	ProgressFill->SetVisibility(Progress > 0.f ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
