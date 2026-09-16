#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Interaction/InteractPrompt.h"

#include "InteractPromptWidget.generated.h"

class UTextBlock;
class USizeBox;
class UImage;
class UMaterialInstanceDynamic;

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

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> ProgressFill;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<USizeBox> PressProgressBarBox;

  private:
	// ProgressFill의 M_SquareProgress를 런타임에 파라미터 바꿀수있게 복사해둔 인스턴스
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ProgressMat;
};
