// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/LoadingScreenWidget.h"

#include "Components/Image.h"
#include "GameFlow/Manager/LoadingScreenManager.h"
#include "Logging/BOLog.h"
#include "Components/TextBlock.h"

void ULoadingScreenWidget::SetLoadingProgressBar(float Percent)
{
	OnLoadingProgressChanged(Percent);
}

void ULoadingScreenWidget::SetLoadingImage(TObjectPtr<UTexture2D> Image)
{
	if (LoadingImage)
	{
		LoadingImage->SetBrushFromTexture(Image);
		OnLoadingImageChanged();
	}
}

void ULoadingScreenWidget::SetLoadingTip(const FString& Tip)
{
	if (LoadingTipText)
	{
		LoadingTipText->SetText(FText::FromString(Tip));
		OnLoadingTipChanged();	
	}
}
