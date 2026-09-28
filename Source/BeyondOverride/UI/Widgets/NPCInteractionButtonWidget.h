// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Enums/NPCEnums.h"
#include "GameFlow/BODelegates.h"

#include "NPCInteractionButtonWidget.generated.h"

class UButton;
class UTextBlock;
class UItemInstanceBase;
class ANPCBase;

UCLASS()
class BEYONDOVERRIDE_API UNPCInteractionButtonWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	void SetNPC(TObjectPtr<ANPCBase> InNPC);
	void SetButtonOption(ENPCInteractionOption InOption);
	void SetButtonSituation(EDialogueSituation InSituation);
	void SetNextDialogueID(FName ID);
	void SetButtonText(FText Text);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void OnButtonClicked();

	void OnInteractionClicked();
	void OnTalkSituation();

	void Talk();
	void OpenShop();
	void Goodbye();

  public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InteractionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonTextBlock;

  private:
	UPROPERTY()
	TObjectPtr<ANPCBase> NPC;
	ENPCInteractionOption Option;
	EDialogueSituation Situation;
	FName NextDialogueID;

	UPROPERTY()
	TArray<UItemInstanceBase*> ShopItems;

  public:
	FOnInteractionButtonClicked OnInteractionButtonClicked;
	FOnTalkButtonClicked OnTalkButtonClicked;
};
