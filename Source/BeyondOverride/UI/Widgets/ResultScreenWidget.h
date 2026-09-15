#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ResultScreenWidget.generated.h"

class UButton;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class BEYONDOVERRIDE_API UResultScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

  private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Result;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> DeadResult;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SurvivalTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillerMonster;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KilledMonster;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OKBtn;

  public:
	UFUNCTION()
	void OnOKBtnClicked();

  public:
	void SetResult();
	void SetDeadResult(ESlateVisibility InVisibility);
	void SetSurvivalTime();
	void SetKillerMonster();
	void SetKilledMonster();
};
