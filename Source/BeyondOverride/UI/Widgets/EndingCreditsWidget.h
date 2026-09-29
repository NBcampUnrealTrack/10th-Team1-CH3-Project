// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "EndingCreditsWidget.generated.h"

class UTextBlock;
class USoundBase;

UCLASS()
class BEYONDOVERRIDE_API UEndingCreditsWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(
		const FGeometry& InGeometry,
		const FKeyEvent& InKeyEvent) override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnCreditsAnimationFinished();

	void SetEndTextBlockVisibility(ESlateVisibility InVisibility);

	void End();

  public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CreditsAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EndAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EndTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> EndingBGM;

  private:
	bool IsInputEnabled;
};
