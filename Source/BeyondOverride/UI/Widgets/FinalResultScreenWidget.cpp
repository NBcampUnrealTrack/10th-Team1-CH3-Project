// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/FinalResultScreenWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Manager/UIManager.h"

void UFinalResultScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OKBtn)
	{
		OKBtn->OnClicked.AddDynamic(this, &UFinalResultScreenWidget::OnOKBtnClicked);
	}

	SetTotalSurvivalTime();
	SetTotalKilledMonster();
	SetFarmingCount();
	SetDeathCount();
}

void UFinalResultScreenWidget::OnOKBtnClicked()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->PopScreen();
	}

	if (ABOGameMode* GM = Cast<ABOGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->End();
	}
}

void UFinalResultScreenWidget::SetTotalSurvivalTime()
{
	if (!GetWorld() || !TotalSurvivalTime)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	int32 Time = FMath::FloorToInt(GameInstance->GetTotalSurvivalTime());
	int32 Minute = Time / 60;
	int32 Second = Time % 60;

	TotalSurvivalTime->SetText(FText::FromString(FString::Format(TEXT("{0}:{1}"), {Minute, Second})));
}

void UFinalResultScreenWidget::SetTotalKilledMonster()
{
	if (!GetWorld() || !TotalKilledMonster)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	TMap<FName, int32> Monsters{};
	GameInstance->GetTotalKilledMonsters(Monsters);
	int32 Count = Monsters.Num();

	TotalKilledMonster->SetText(FText::FromString(FString::FromInt(Count)));
}

void UFinalResultScreenWidget::SetFarmingCount()
{
	if (!GetWorld() || !FarmingCount)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	FarmingCount->SetText(FText::FromString(FString::FromInt(GameInstance->GetFarmingCount())));
}

void UFinalResultScreenWidget::SetDeathCount()
{
	if (!GetWorld() || !DeathCount)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	DeathCount->SetText(FText::FromString(FString::FromInt(GameInstance->GetDeathCount())));
}
