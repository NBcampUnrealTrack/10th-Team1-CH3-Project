// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "FinalResultScreenWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class BEYONDOVERRIDE_API UFinalResultScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	virtual void NativeConstruct() override;

  private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalSurvivalTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalKilledMonster;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FarmingCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DeathCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OKBtn;

  public:
	UFUNCTION()
	void OnOKBtnClicked();

  public:
	void SetTotalSurvivalTime();
	void SetTotalKilledMonster();
	void SetFarmingCount();
	void SetDeathCount();
	// total killed monster
};
