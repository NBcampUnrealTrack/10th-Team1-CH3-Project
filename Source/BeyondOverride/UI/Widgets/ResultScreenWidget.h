#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "ResultScreenWidget.generated.h"

class UButton;
class UTextBlock;
class UVerticalBox;
class UKillCountEntryWidget;
class UItemSlotPanelWidget;
class UImage;

UCLASS()
class BEYONDOVERRIDE_API UResultScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UResultScreenWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Result")
	bool bIsSurvived;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	TObjectPtr<UDataTable> MonsterDataTable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MonsterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OKBtn;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UTextBlock> KillerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> KillCountList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SurvivalTimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KilledMonsterText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotPanelWidget> BackpackSlotPanel;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UKillCountEntryWidget> KillCountEntryClass;

	UFUNCTION()
	void OnOKBtnClicked();
};
