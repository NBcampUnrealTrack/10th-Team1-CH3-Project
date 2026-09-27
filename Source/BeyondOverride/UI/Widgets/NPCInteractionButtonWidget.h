// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Enums/NPCEnums.h"

#include "NPCInteractionButtonWidget.generated.h"

class UButton;
class UTextBlock;
class UItemInstanceBase;

UCLASS()
class BEYONDOVERRIDE_API UNPCInteractionButtonWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

	void SetNPCID(FName ID);
	void SetButtonOption(ENPCInteractionOption InOption);
	void SetButtonText(FText Text);

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void OnInteractionButtonClicked();

	void OpenShop();
	void Goodbye();

  public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InteractionButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonTextBlock;

  private:
	FName NPCID;
	ENPCInteractionOption Option;

	UPROPERTY()
	TArray<UItemInstanceBase*> ShopItems;
};
