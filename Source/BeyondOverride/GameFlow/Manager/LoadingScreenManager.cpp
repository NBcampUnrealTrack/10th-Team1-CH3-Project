// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/LoadingScreenManager.h"

#include "Algo/RandomShuffle.h"
#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"

void ULoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!GetWorld())
	{
		return;
	}

	if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
	{
		if (UBODataAsset* DataAsset = GameInstance->GetBODataAsset())
		{
			LoadingScreenWidgetClass = DataAsset->GetLoadingScreenWidgetClass();
			DataAsset->GetLoadingImages(LoadingImages);

			UpdateTime = DataAsset->GetLoadingScreenUpdateTime();
			ImageChangeTime = DataAsset->GetLoadingImageChangeTime();
			ImageUpdateInterval = DataAsset->GetLoadingImageUpdateInterval();
			ProgressUpdateInterval = DataAsset->GetLoadingProgressUpdateInterval();
		}
	}

	ImageCount = LoadingImages.Num();

	InitSetting();
}

void ULoadingScreenManager::InitSetting()
{
	CurUpdateTime = UpdateTime;
	CurImageTime = 0.0f;
	CurProgress = 0.05f;

	ImageIndex = 0;

	Algo::RandomShuffle(LoadingImages);
}

void ULoadingScreenManager::ShowLoadingScreenWidget(bool IsNew)
{
	if (!GetWorld())
	{
		return;
	}

	if (IsNew)
	{
		InitSetting();
	}
	else
	{
		CurUpdateTime = GetWorld()->GetTimerManager().GetTimerRemaining(UpdateTimer);
	}

	GetWorld()->GetTimerManager().ClearTimer(UpdateTimer);

	HideLoadingScreenWidget();

	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
		LoadingScreenWidget->SetLoadingProgressBar(CurProgress);
		LoadingScreenWidget->SetLoadingImage(LoadingImages[ImageIndex]);
		LoadingScreenWidget->AddToViewport();

		GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &ULoadingScreenManager::UpdateLoadingScreenWidget, CurUpdateTime, true);
	}
}

void ULoadingScreenManager::HideLoadingScreenWidget()
{
	if (LoadingScreenWidget.IsValid())
	{
		LoadingScreenWidget->RemoveFromParent();
		LoadingScreenWidget = nullptr;
	}
}

void ULoadingScreenManager::UpdateLoadingScreenWidget()
{
	if (LoadingScreenWidget.IsValid())
	{
		CurUpdateTime = UpdateTime;
		CurImageTime += ImageUpdateInterval;
		CurProgress = FMath::Min(CurProgress + ProgressUpdateInterval, 0.95f);

		LoadingScreenWidget->SetLoadingProgressBar(CurProgress);

		if (CurImageTime >= ImageChangeTime)
		{
			CurImageTime = 0.0f;
			ImageIndex = (ImageIndex + 1) % ImageCount;

			LoadingScreenWidget->SetLoadingImage(LoadingImages[ImageIndex]);
		}
	}
}
