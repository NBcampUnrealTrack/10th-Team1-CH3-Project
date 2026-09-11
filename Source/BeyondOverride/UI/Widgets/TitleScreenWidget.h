#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "TitleScreenWidget.generated.h"

class UButton;

UCLASS()
class BEYONDOVERRIDE_API UTitleScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (StartBtn))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (ExitBtn))
	TObjectPtr<UButton> ExitButton;
};
