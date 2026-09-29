// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/EndingCreditsWidget.h"

#include "Components/TextBlock.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
#include "UI/Manager/UIManager.h"

void UEndingCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IsInputEnabled = false;

	SetEndTextBlockVisibility(ESlateVisibility::Collapsed);

	FWidgetAnimationDynamicEvent Event;
	Event.BindUFunction(this, FName("OnCreditsAnimationFinished"));

	BindToAnimationFinished(CreditsAnimation, Event);

	PlayAnimation(CreditsAnimation);

	if (EndingBGM)
	{
		UGameplayStatics::PlaySound2D(this, EndingBGM);
	}

	SetIsFocusable(true);
	SetKeyboardFocus();
}

FReply UEndingCreditsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogGameFlow, Warning, TEXT("Native On Key Down Called"));

	if (!IsInputEnabled)
	{
		return FReply::Unhandled();
	}

	End();

	return FReply::Handled();
}

FReply UEndingCreditsWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogGameFlow, Warning, TEXT("NativeOnMouseButtonDown Called"));
	if (!IsInputEnabled)
	{
		return FReply::Unhandled();
	}

	End();

	return FReply::Handled();
}

void UEndingCreditsWidget::OnCreditsAnimationFinished()
{
	IsInputEnabled = true;

	SetEndTextBlockVisibility(ESlateVisibility::Visible);
	PlayAnimation(EndAnimation);
}

void UEndingCreditsWidget::SetEndTextBlockVisibility(ESlateVisibility InVisibility)
{
	if (EndTextBlock)
	{
		EndTextBlock->SetVisibility(InVisibility);
	}
}

void UEndingCreditsWidget::End()
{
	UE_LOG(LogGameFlow, Warning, TEXT("End Called"));
	if (!GetWorld())
	{
		return;
	}

	if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("EndGame Called"));
		GameMode->EndGame();
	}

	if (UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>())
	{
		UIManager->PopScreen();
	}
}
