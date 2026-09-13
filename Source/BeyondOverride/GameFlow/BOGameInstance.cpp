// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameInstance.h"

#include "BOGameMode.h"
#include "BOWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"

void UBOGameInstance::Init()
{
	Super::Init();

	BODataAsset = nullptr;
	Levels.Empty();
	Regions.Empty();
	BasicEquipments.Empty();
	ExitActivateProb = 0.5f;

	LoadMonsterData();

	InitSetting();
}

void UBOGameInstance::LoadMonsterData()
{
	if (!BODataAsset)
	{
		return;
	}

	/*if (UDataTable* AIData = BODataAsset->GetMonsterDataTable())
	{
		TArray<FAIData*> AllRows{};
		AIData->GetAllRows<FAIData>(TEXT("Get All AI Datas"), AllRows);

		for (FAIData* Row : AllRows)
		{
			if (Row)
			{
				FName Id = Row->Id;
				AIDatas.Add(Id, *Row);
			}
		}
	}*/
}

void UBOGameInstance::InitSetting()
{
	GameState = EGameState::Begin;
	PlayingState = EPlayingState::None;
	FarmingResult = EFarmingResult::None;

	TotalSurvivalTime = 0.0f;
	SurvivalTime = 0.0f;
	TotalKilledMonsters.Empty();
	KilledMonsters.Empty();
	KillerMonster = "None";

	CurHealth = 0;
	CurShield = 0;
	TotalMoney = 0;
	// Inventory.Empty();

	IsKeyCardAcquired = false;

	// MonsterDatas.Empty();

	OpenLevel(ELevel::Bunker);
	StartFarming(); // test code
}

void UBOGameInstance::Start()
{
	GameState = EGameState::Playing;
	PlayingState = EPlayingState::Bunker;
}

void UBOGameInstance::Restart()
{
	InitSetting();
}

void UBOGameInstance::Exit()
{
	if (GetWorld())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController<ABOPlayerController>(), EQuitPreference::Quit, false);
	}
}

void UBOGameInstance::StartFarming()
{
	GameState = EGameState::Playing; // test code
	PlayingState = EPlayingState::Farming;

	SurvivalTime = 0.0f;
	KilledMonsters.Empty();
	KillerMonster = "None";

	OpenLevel(ELevel::Main);
}

void UBOGameInstance::EndFarming(EFarmingResult Result)
{
	PlayingState = EPlayingState::Bunker;
	FarmingResult = Result;

	SaveFarmingData();

	OpenLevel(ELevel::Bunker);
}

void UBOGameInstance::OpenLevel(ELevel Level)
{
	SavePlayerData();

	if (GetWorld() && Levels.Contains(Level))
	{
		UGameplayStatics::OpenLevel(GetWorld(), Levels[Level]);

		if (Level == ELevel::Main)
		{
			UE_LOG(LogTemp, Warning, TEXT("Main"));
		}
		else if (Level == ELevel::Bunker)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bunker"));
		}
	}
}

void UBOGameInstance::SavePlayerData()
{
	if (ABOCharacter* Character = Cast<ABOCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		// health, shield
		if (UStatComponent* StatComponent = Character->GetStatComponent())
		{
			CurHealth = StatComponent->GetCurHealth();
			CurShield = StatComponent->GetCurShield();
		}

		// money, inventory
		// search key card in the inventory
	}
}

void UBOGameInstance::SaveFarmingData()
{
	if (!GetWorld())
	{
		return;
	}

	// survival time
	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		SurvivalTime = WorldSubsystem->GetSurvivalTime();

		TotalSurvivalTime += SurvivalTime;

		UE_LOG(LogTemp, Warning, TEXT("Survival Time : %f"), SurvivalTime);
		UE_LOG(LogTemp, Warning, TEXT("Total Survival Time : %f"), TotalSurvivalTime);
	}

	// killed monsters / killer monster
	if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
	{
		GameMode->GetKilledMonsters(KilledMonsters);
		KillerMonster = GameMode->GetKillerMonster();

		for (TPair<FName, int32> Monster : KilledMonsters)
		{
			FName Id = Monster.Key;
			int32 Count = Monster.Value;

			if (TotalKilledMonsters.Contains(Id))
			{
				TotalKilledMonsters[Id] += Count;
			}
			else
			{
				TotalKilledMonsters.Add(Id, Count);
			}
		}
	}
}

UBODataAsset* UBOGameInstance::GetBODataAsset() const
{
	return BODataAsset;
}

void UBOGameInstance::GetLevels(TMap<ELevel, FName>& Data) const
{
	Data = Levels;
}

void UBOGameInstance::GetRegions(TArray<FName>& Data) const
{
	Data = Regions;
}

void UBOGameInstance::GetBasicEquipments(TArray<FName>& Data) const
{
	Data = BasicEquipments;
}

float UBOGameInstance::GetExitActivateProb() const
{
	return ExitActivateProb;
}

EGameState UBOGameInstance::GetGameState() const
{
	return GameState;
}

EPlayingState UBOGameInstance::GetPlayingState() const
{
	return PlayingState;
}

EFarmingResult UBOGameInstance::GetFarmingResult() const
{
	return FarmingResult;
}

float UBOGameInstance::GetTotalSurvivalTime() const
{
	return TotalSurvivalTime;
}

float UBOGameInstance::GetSurvivalTime() const
{
	return SurvivalTime;
}

void UBOGameInstance::GetTotalKilledMonsters(TMap<FName, int32>& Data) const
{
	Data = TotalKilledMonsters;
}

void UBOGameInstance::GetKilledMonsters(TMap<FName, int32>& Data) const
{
	Data = KilledMonsters;
}

FName UBOGameInstance::GetKillerMonster() const
{
	return KillerMonster;
}

float UBOGameInstance::GetCurrentHealth() const
{
	return CurHealth;
}

float UBOGameInstance::GetCurrentShield() const
{
	return CurShield;
}

int32 UBOGameInstance::GetTotalMoney() const
{
	return TotalMoney;
}

bool UBOGameInstance::GetIsCardAcquired() const
{
	return IsKeyCardAcquired;
}
