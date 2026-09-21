// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/LoadingScreenManager.h"

#include "MoviePlayer.h"
#include "ShaderCompiler.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"

void ULoadingScreenManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();

	if (GameInstance)
	{
		if (UBODataAsset* DataAsset = GameInstance->GetBODataAsset())
		{
			LoadingScreenWidgetClass = DataAsset->GetLoadingScreenWidgetClass();
		}
	}

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenManager::OnPostLoadMap);
}

void ULoadingScreenManager::ShowLoadingScreenWidget()
{
	if (!GameInstance)
	{
		return;
	}

	if (LoadingScreenWidget)
	{
		LoadingScreenWidget = nullptr;
	}

	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<ULoadingScreenWidget>(GameInstance, LoadingScreenWidgetClass);

		FLoadingScreenAttributes LoadingScreenAttributes{};

		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreenAttributes.bWaitForManualStop = true;

		TSharedPtr<SWidget> SlateWidget = LoadingScreenWidget->TakeWidget();
		LoadingScreenAttributes.WidgetLoadingScreen = SlateWidget;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

		GetMoviePlayer()->OnMoviePlaybackTick().RemoveAll(this);
		GetMoviePlayer()->OnMoviePlaybackTick().AddUObject(this, &ULoadingScreenManager::UpdateLoadingScreenWidget);

		UE_LOG(LogGameFlow, Warning,
			   TEXT("SetupLoadingScreen: AutoComplete=%d, WaitForManualStop=%d"),
			   LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes,
			   LoadingScreenAttributes.bWaitForManualStop);

		UE_LOG(LogGameFlow, Warning,
			   TEXT("After Setup - Playing: %d"),
			   GetMoviePlayer()->IsMovieCurrentlyPlaying());
	}
}

void ULoadingScreenManager::OnPostLoadMap(UWorld* World)
{
	FramesToWait = 15;
	FCoreDelegates::OnEndFrame.AddUObject(this, &ULoadingScreenManager::OnEndFrame);
}

void ULoadingScreenManager::OnEndFrame()
{
	if (--FramesToWait <= 0 && IsRenderingReady())
	{
		FCoreDelegates::OnEndFrame.RemoveAll(this);
		GetMoviePlayer()->StopMovie();
	}
}

bool ULoadingScreenManager::IsRenderingReady() const
{
	if (GShaderCompilingManager && GShaderCompilingManager->IsCompiling())
		return false;

	UWorld* World = GetWorld();
	if (!World || !World->AreActorsInitialized() || !World->HasBegunPlay())
		return false;

	// 텍스처 스트리밍이 아직 진행 중이면 대기
	if (IStreamingManager::Get().IsStreamingEnabled())
	{
		if (IStreamingManager::Get().StreamAllResources(0.0f) > 0.0f)
			return false;
	}

	return true;
}

void ULoadingScreenManager::UpdateLoadingScreenWidget(float DeltaTime)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // Key
			3.0f,        // 표시 시간
			FColor::Red, // 색상
			TEXT("After Setup - Playing: %d"),
			GetMoviePlayer()->IsMovieCurrentlyPlaying());
	}

	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->UpdateLoading(DeltaTime);
	}
}

void ULoadingScreenManager::HideLoadingScreenWidget()
{
	UE_LOG(LogGameFlow, Warning, TEXT("========== FinishLoading / StopMovie =========="));

	if (GetMoviePlayer())
	{
		UE_LOG(LogGameFlow, Warning,
			   TEXT("LoadingFinished: %d"),
			   GetMoviePlayer()->IsLoadingFinished());

		UE_LOG(LogGameFlow, Warning,
			   TEXT("MovieCurrentlyPlaying: %d"),
			   GetMoviePlayer()->IsMovieCurrentlyPlaying());

		UE_LOG(LogGameFlow, Warning, TEXT("Stop"));
		GetMoviePlayer()->StopMovie();

		UE_LOG(LogGameFlow, Warning,
			   TEXT("LoadingFinished: %d"),
			   GetMoviePlayer()->IsLoadingFinished());

		UE_LOG(LogGameFlow, Warning,
			   TEXT("MovieCurrentlyPlaying: %d"),
			   GetMoviePlayer()->IsMovieCurrentlyPlaying());
	}

	if (LoadingScreenWidget)
	{
		LoadingScreenWidget = nullptr;
	}
}
