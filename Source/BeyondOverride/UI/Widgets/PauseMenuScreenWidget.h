#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "PauseMenuScreenWidget.generated.h"

class UButton;

UCLASS()
class BEYONDOVERRIDE_API UPauseMenuScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitBtn;
};
