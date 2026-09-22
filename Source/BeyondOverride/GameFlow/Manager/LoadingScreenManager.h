// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widgets/LoadingScreenWidget.h"

#include "LoadingScreenManager.generated.h"

UCLASS()
class BEYONDOVERRIDE_API ULoadingScreenManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  public:
	void InitSetting();

	void ShowLoadingScreenWidget(bool IsNew);
	void HideLoadingScreenWidget();

	UFUNCTION(BlueprintCallable)
	void UpdateLoadingScreenWidget();

  private:
	float UpdateTime;
	float ImageChangeTime;
	float ImageUpdateInterval;
	float ProgressUpdateInterval;
	int32 ImageCount;

	float CurUpdateTime;
	float CurImageTime;
	float CurProgress;
	int32 ImageIndex;

	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;
	TWeakObjectPtr<ULoadingScreenWidget> LoadingScreenWidget;
	TArray<TObjectPtr<UTexture2D>> LoadingImages;

	FTimerHandle UpdateTimer;
};
