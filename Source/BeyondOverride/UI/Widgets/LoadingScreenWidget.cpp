// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/LoadingScreenWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GameFlow/Manager/LoadingScreenManager.h"
#include "Logging/BOLog.h"

void ULoadingScreenWidget::SetLoadingProgressBar(float Percent)
{
	if (LoadingProgressBar)
	{
		LoadingProgressBar->SetPercent(Percent);
	}
}

void ULoadingScreenWidget::SetLoadingImage(TObjectPtr<UTexture2D> Image)
{
	if (LoadingImage)
	{
		LoadingImage->SetBrushFromTexture(Image);
	}
}
