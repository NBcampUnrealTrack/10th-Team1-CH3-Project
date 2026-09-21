// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/LoadingScreenWidget.h"

#include "Algo/RandomShuffle.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Logging/BOLog.h"

void ULoadingScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurProgress = 0.0f;
	ImageChangeTime = 0.0f;
	ImageCount = LoadingImages.Num();
	ImageIndex = 0;

	SetRandomLoadingImages();
	SetLoadingImage(ImageIndex);
}

void ULoadingScreenWidget::UpdateLoading(float DeltaTime)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // Key
			3.0f,        // 표시 시간
			FColor::Red, // 색상
			TEXT("Update Loading"));
	}

	if (LoadingProgressBar)
	{
		CurProgress += DeltaTime * 0.2f;
		CurProgress = FMath::Min(CurProgress, 0.9f);

		SetLoadingProgressBar(CurProgress);
	}

	if (LoadingImage)
	{
		ImageChangeTime += DeltaTime;

		if (ImageChangeTime >= 3.0f)
		{
			ImageChangeTime = 0.0f;

			ImageIndex = (ImageIndex + 1) % ImageCount;

			SetLoadingImage(ImageIndex);
		}
	}
}

void ULoadingScreenWidget::SetRandomLoadingImages()
{
	Algo::RandomShuffle(LoadingImages);
}

void ULoadingScreenWidget::SetLoadingProgressBar(float Percent)
{
	if (LoadingProgressBar)
	{
		LoadingProgressBar->SetPercent(Percent);
	}
}

void ULoadingScreenWidget::SetLoadingImage(int32 Index)
{
	if (LoadingImage)
	{
		TObjectPtr<UTexture2D> Image = LoadingImages[Index];
		LoadingImage->SetBrushFromTexture(Image);
	}
}
