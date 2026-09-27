// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Enums/NPCEnums.h"

#include "NPCInteractionWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UNPCInteractionButtonWidget;

UCLASS()
class BEYONDOVERRIDE_API UNPCInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	void AddButton(UWidget* Button);
	TObjectPtr<UNPCInteractionButtonWidget> CreateButtonWidget(FName NPCID, ENPCInteractionOption Option);
	void SetDialogue(FText Dialogue);

  public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ButtonVerticalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueTextBlock;

  public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UNPCInteractionButtonWidget> NPCInteractionButtonWidgetClass;
};
