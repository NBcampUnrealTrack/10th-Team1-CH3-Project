// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "LoadingScreenWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void SetLoadingProgressBar(float Percent);
	void SetLoadingImage(TObjectPtr<UTexture2D> Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Loading")
	void OnLoadingTipChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Loading")
	void OnLoadingImageChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Loading")
	void OnLoadingProgressChanged(float Percent);

	void SetLoadingTip(const FString& Tip);

  public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> LoadingImage;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> LoadingTipText;
};
