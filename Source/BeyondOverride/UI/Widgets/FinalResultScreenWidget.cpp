// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/FinalResultScreenWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Manager/UIManager.h"
#include "UI/Widgets/KillCountEntryWidget.h"

UFinalResultScreenWidget::UFinalResultScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UKillCountEntryWidget> KillCountEntryWBP(TEXT("/Game/UI/Widgets/WBP_KillCountEntry"));
	if (KillCountEntryWBP.Succeeded())
	{
		KillCountEntryClass = KillCountEntryWBP.Class;
	}
}

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
		GM->ShowEnding();
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

	float SurvivalTime = GameInstance->GetTotalSurvivalTime();
	int32 Hours = FMath::FloorToInt(SurvivalTime / 3600.0f);
	int32 Minutes = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 3600.0f) / 60.0f);
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(SurvivalTime, 60.0f));

	FString TimeString;

	if (Hours)
	{
		TimeString = FString::Printf(TEXT("%d:%02d:%02d"), Hours, Minutes, Seconds);
	}
	else
	{
		TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	}

	TotalSurvivalTime->SetText(FText::FromString(TimeString));
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
	int32 TotalKilled = 0;
	for (auto& Elem : Monsters)
		TotalKilled += Elem.Value;

	TotalKilledMonster->SetText(FText::FromString(FString::FromInt(TotalKilled)));

	if (KillCountList && KillCountEntryClass)
	{
		KillCountList->ClearChildren();

		for (const TPair<FName, int32>& Pair : Monsters)
		{
			UKillCountEntryWidget* Entry = CreateWidget<UKillCountEntryWidget>(this, KillCountEntryClass);
			if (!Entry)
				return;

			Entry->SetKillCountEntry(FText::FromName(Pair.Key), Pair.Value);
			KillCountList->AddChildToVerticalBox(Entry);
		}
	}
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
