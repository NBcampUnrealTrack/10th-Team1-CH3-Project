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
	virtual void NativeConstruct() override;

	void UpdateLoading(float DeltaTime);

  private:
	void SetRandomLoadingImages();
	void SetLoadingProgressBar(float Percent);
	void SetLoadingImage(int32 Index);

  public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* LoadingProgressBar;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UImage* LoadingImage;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TArray<TObjectPtr<UTexture2D>> LoadingImages;

  private:
	float CurProgress;
	float ImageChangeTime;
	int32 ImageCount;
	int32 ImageIndex;
};
