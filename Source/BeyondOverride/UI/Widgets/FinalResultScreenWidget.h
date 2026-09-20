// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "FinalResultScreenWidget.generated.h"

class UButton;
class UTextBlock;
class UKillCountEntryWidget;
class UVerticalBox;
class UImage;

UCLASS()
class BEYONDOVERRIDE_API UFinalResultScreenWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	UFinalResultScreenWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

  private:
	UPROPERTY(EditDefaultsOnly, Category = "Monster")
	TObjectPtr<UDataTable> MonsterDataTable;

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> KillCountList;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UKillCountEntryWidget> KillCountEntryClass;

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
