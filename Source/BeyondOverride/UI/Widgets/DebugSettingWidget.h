// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "DebugSettingWidget.generated.h"

class UComboBoxString;
class UCheckBox;
class UEditableTextBox;
class UButton;

UCLASS()
class BEYONDOVERRIDE_API UDebugSettingWidget : public UUserWidget
{
	GENERATED_BODY()

  private:
	virtual void NativeConstruct() override;

  public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> LocationComboBoxString;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> BossDefeatCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> SprintEditableTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OKButton;

  private:
	void SetLocationList();
	void SetLocationComboBoxString();

	UFUNCTION()
	void OnOKButtonClicked();

	void OnSetLocation();
	void OnSetBossDefeated();
	void OnSetCharacterSpeed();

  private:
	TMap<FName, TPair<FVector, FRotator>> Locations;
};
