#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "NotificationWidget.generated.h"

class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UFUNCTION(BlueprintCallable, Category = "Notification")
	void ShowNotification(const FText& Main, const FText& Sub, float Duration);

	UFUNCTION(BlueprintImplementableEvent, Category = "Notification", meta = (DisplayName = "On Show Notification"))
	void PlayShowNotification();
	UFUNCTION(BlueprintImplementableEvent, Category = "Notification", meta = (DisplayName = "On Hide Notification"))
	void PlayHideNotification();

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MainText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SubText;

  private:
	UFUNCTION()
	void HideNotification();

	FTimerHandle HideTimerHandle;

	float HideDuration = 3.0f;
};
