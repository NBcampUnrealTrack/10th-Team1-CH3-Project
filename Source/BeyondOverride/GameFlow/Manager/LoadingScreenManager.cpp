// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/LoadingScreenManager.h"

#include "Algo/RandomShuffle.h"
#include "DataAssets/BODataAsset.h"
#include "DataTables/UI/LoadingTipDataRow.h"
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

			if (UDataTable* TipTable = DataAsset->GetLoadingTipTable())
			{
				TArray<FLoadingTipDataRow*> Rows;
				TipTable->GetAllRows<FLoadingTipDataRow>(TEXT("LoadingTip"), Rows);

				for (const FLoadingTipDataRow* Row : Rows)
					if (Row)
						LoadingTips.Add(Row->LoadingTip);
			}

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
	CurProgress = 0.0f;

	ImageIndex = 0;
	TipIndex = 0;

	Algo::RandomShuffle(LoadingImages);
	Algo::RandomShuffle(LoadingTips);

	NextLoadingTip();
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
	GetWorld()->GetTimerManager().ClearTimer(HideTimer);
	RemoveLoadingScreenWidget();

	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<ULoadingScreenWidget>(GetWorld(), LoadingScreenWidgetClass);
		LoadingScreenWidget->SetLoadingProgressBar(CurProgress);
		LoadingScreenWidget->SetLoadingImage(LoadingImages[ImageIndex]);
		LoadingScreenWidget->SetLoadingTip(CurLoadingTip);
		LoadingScreenWidget->AddToViewport();

		GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &ULoadingScreenManager::UpdateLoadingScreenWidget, CurUpdateTime, true);
	}
}

void ULoadingScreenManager::HideLoadingScreenWidget()
{
	if (!LoadingScreenWidget.IsValid())
	{
		OnLoadingScreenHidden.Broadcast();
		return;
	}

	if (!GetWorld())
	{
		RemoveLoadingScreenWidget();
		OnLoadingScreenHidden.Broadcast();
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(UpdateTimer);

	CurProgress = 1.0f;
	LoadingScreenWidget->SetLoadingProgressBar(CurProgress);

	GetWorld()->GetTimerManager().SetTimer(HideTimer, this, &ULoadingScreenManager::OnHideTimerFinished, HideDelay, false);
}

void ULoadingScreenManager::RemoveLoadingScreenWidget()
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
		CurProgress = FMath::Min(CurProgress + ProgressUpdateInterval, 0.99f);

		LoadingScreenWidget->SetLoadingProgressBar(CurProgress);

		if (CurImageTime >= ImageChangeTime)
		{
			CurImageTime = 0.0f;
			ImageIndex = (ImageIndex + 1) % ImageCount;

			LoadingScreenWidget->SetLoadingImage(LoadingImages[ImageIndex]);
			NextLoadingTip();
			LoadingScreenWidget->SetLoadingTip(CurLoadingTip);
		}
	}
}

void ULoadingScreenManager::NextLoadingTip()
{
	if (LoadingTips.IsEmpty())
	{
		CurLoadingTip.Empty();
		return;
	}

	CurLoadingTip = LoadingTips[TipIndex];
	TipIndex = (TipIndex + 1) % LoadingTips.Num();
}

void ULoadingScreenManager::OnHideTimerFinished()
{
	RemoveLoadingScreenWidget();
	OnLoadingScreenHidden.Broadcast();
}
