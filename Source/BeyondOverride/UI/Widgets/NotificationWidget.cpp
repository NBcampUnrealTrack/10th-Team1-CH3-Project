#include "UI/Widgets/NotificationWidget.h"
#include "Components/TextBlock.h"

void UNotificationWidget::ShowNotification(const FText& Main, const FText& Sub, float Duration)
{
	if (MainText)
	{
		MainText->SetText(Main);
	}

	if (SubText)
	{
		SubText->SetText(Sub);
	}

	AddToViewport();

	GetWorld()->GetTimerManager().SetTimer(
		HideTimerHandle,
		this,
		&UNotificationWidget::HideNotification,
		Duration,
		false);

	PlayShowNotification();
}


void UNotificationWidget::HideNotification()
{
	PlayHideNotification();
}
