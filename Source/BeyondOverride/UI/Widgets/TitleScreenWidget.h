#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "TitleScreenWidget.generated.h"

class UButton;
class USoundBase;

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitBtn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundBase> StartBGM;
};
