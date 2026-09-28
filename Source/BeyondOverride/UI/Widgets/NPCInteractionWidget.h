// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "DataTables/NPC/NPCDialogueData.h"
#include "Enums/NPCEnums.h"

#include "NPCInteractionWidget.generated.h"

class UDialogueManager;
class UBorder;
class UVerticalBox;
class UTextBlock;
class UNPCInteractionButtonWidget;
class ANPCBase;

UCLASS()
class BEYONDOVERRIDE_API UNPCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	void InitSetting();

	void AddButton(UWidget* Button);
	TObjectPtr<UNPCInteractionButtonWidget> CreateButtonWidget(ENPCInteractionOption Option);
	TObjectPtr<UNPCInteractionButtonWidget> CreateButtonWidget(EDialogueSituation Situation, FName NextDialogueID, FText Text);

	void GetDialogueInteractionData(EDialogueSituation DialogueSituation);
	void SetDialogueTextBlock(FText Dialogue);

	void StartTalk();
	void ShowTalkList();
	void ShowQuestList();
	void HealPlayer();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void OnDialogueBorderClicked();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void OnInteractionButtonClicked(ENPCInteractionOption Option);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void OnTalkSituation(FName TopicID);

  public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> DialogueBorder;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonVerticalBox;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UNPCInteractionButtonWidget> NPCInteractionButtonWidgetClass;

  private:
	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;

	TObjectPtr<ANPCBase> NPC;

	FDialogueInteractionData DialogueInteractionData;
	FDialogueData DialogueData;
};
