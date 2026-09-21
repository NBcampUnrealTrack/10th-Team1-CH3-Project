// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widgets/LoadingScreenWidget.h"

#include "LoadingScreenManager.generated.h"

class UBOGameInstance;

UCLASS()
class BEYONDOVERRIDE_API ULoadingScreenManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  public:
	void ShowLoadingScreenWidget();

	UFUNCTION(BlueprintCallable)
	void OnPostLoadMap(UWorld* World);

	UFUNCTION(BlueprintCallable)
	void OnEndFrame();

	bool IsRenderingReady() const;

	UFUNCTION(BlueprintCallable)
	void UpdateLoadingScreenWidget(float DeltaTime);

	void HideLoadingScreenWidget();

  private:
	UBOGameInstance* GameInstance;

	float FramesToWait;

	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;
	TObjectPtr<ULoadingScreenWidget> LoadingScreenWidget;
};
