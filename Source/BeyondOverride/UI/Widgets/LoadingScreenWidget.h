// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "LoadingScreenWidget.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class BEYONDOVERRIDE_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	void SetLoadingProgressBar(float Percent);
	void SetLoadingImage(TObjectPtr<UTexture2D> Image);

  public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> LoadingProgressBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> LoadingImage;
};
