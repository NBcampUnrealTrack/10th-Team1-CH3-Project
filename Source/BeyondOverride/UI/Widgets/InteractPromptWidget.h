#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Interaction/InteractPrompt.h"

#include "InteractPromptWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class BEYONDOVERRIDE_API UInteractPromptWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UFUNCTION()
	void HandleFocusChanged(bool bHasTarget, FInteractPrompt Data);
	UFUNCTION()
	void HandleHoldProgress(float Progress);

  protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HoldBar;
};
