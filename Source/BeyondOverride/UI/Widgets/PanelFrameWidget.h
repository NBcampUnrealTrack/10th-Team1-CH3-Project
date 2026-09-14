#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "PanelFrameWidget.generated.h"

class UTextBlock;
class UHorizontalBox;

UCLASS()
class BEYONDOVERRIDE_API UPanelFrameWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UFUNCTION(BlueprintCallable, Category = "PanelFrame")
	void SetContainerName(const FText& InName);

	UFUNCTION(BlueprintCallable, Category = "PanelFrame")
	void SetSlotCount(int32 CurrentCount, int32 MaxCount);

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ContainerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CountTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxCountText;
};
