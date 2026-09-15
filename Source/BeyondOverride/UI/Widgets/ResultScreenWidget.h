#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ResultScreenWidget.generated.h"

class UButton;

UCLASS()
class BEYONDOVERRIDE_API UResultScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OKBtn;

	UFUNCTION()
	void OnOKBtnClicked();
};
