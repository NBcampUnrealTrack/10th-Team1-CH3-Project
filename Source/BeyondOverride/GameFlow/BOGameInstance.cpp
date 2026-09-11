// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/ContainerManager.h"
#include "Manager/ExitManager.h"
#include "Manager/SpawnVolumeManager.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"

void UBOGameInstance::Init()
{
	Super::Init();

	GameState = EGameState::Begin;
	PlayingState = EPlayingState::None;
	FarmingResult = EFarmingResult::None;
	TotalSurvivalTime = 0.0f;
	CurrentHealth = 0;
	CurrentShield = 0;
	TotalMoney = 0;
	// Inventory.Empty();

	SpawnVolumeDatas.Empty();
	// AIDatas.Empty();
	ContainerDatas.Empty();

	LoadSpawnVolumeData();
	LoadAIData();
	LoadContainerData();

	OpenLevel(ELevel::Bunker);
	StartFarming();
}

void UBOGameInstance::LoadSpawnVolumeData()
{
	if (!GameDataAsset)
	{
		return;
	}

	if (UDataTable* SpawnVolumeDataTable = GameDataAsset->GetSpawnVolumeDataTable())
	{
		TArray<FSpawnStruct*> AllRows{};
		SpawnVolumeDataTable->GetAllRows<FSpawnStruct>(TEXT("Get All Spawn Volume Datas"), AllRows);

		for (FSpawnStruct* Row : AllRows)
		{
			if (Row)
			{
				FName Id = Row->Id;
				SpawnVolumeDatas.Add(Id, *Row);
			}
		}
	}
}

void UBOGameInstance::LoadAIData()
{
	if (!GameDataAsset)
	{
		return;
	}

	/*if (UDataTable* AIData = GameDataAsset->GetAIDataTable())
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

void UBOGameInstance::LoadContainerData()
{
	if (!GameDataAsset)
	{
		return;
	}

	if (UDataTable* ContainerDataTable = GameDataAsset->GetContainerDataTable())
	{
		TArray<FSpawnStruct*> AllRows{};
		ContainerDataTable->GetAllRows<FSpawnStruct>(TEXT("Get All Container Datas"), AllRows);

		for (FSpawnStruct* Row : AllRows)
		{
			if (Row)
			{
				FName Id = Row->Id;
				ContainerDatas.Add(Id, *Row);
			}
		}
	}
}

void UBOGameInstance::Start()
{
	GameState = EGameState::Playing;
	PlayingState = EPlayingState::Bunker;

	// 기초 장비 지급
}

void UBOGameInstance::Restart()
{
	Init();

	if (USpawnVolumeManager* SpawnVolumeManager = GetSubsystem<USpawnVolumeManager>())
	{
		SpawnVolumeManager->Initialize();
	}

	if (UContainerManager* ContainerManager = GetSubsystem<UContainerManager>())
	{
		ContainerManager->Initialize();
	}

	if (UExitManager* ExitManager = GetSubsystem<UExitManager>())
	{
		ExitManager->Initialize();
	}
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

	OpenLevel(ELevel::Main);
}

void UBOGameInstance::EndFarming(EFarmingResult Result)
{
	PlayingState = EPlayingState::Bunker;
	FarmingResult = Result;

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
	// 플레이어 정보 저장 - 체력, 실드, 돈, 인벤토리
	if (ABOCharacter* Character = Cast<ABOCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
	}
}

// TMap<ELevel, FName> UBOGameInstance::GetLevels() const
//{
//	return Levels;
// }
//
// TArray<FName> UBOGameInstance::GetRegions() const
//{
//	return Regions;
// }

void UBOGameInstance::GetSpawnVolumeData(FName Id, FSpawnStruct& Data)
{
	if (SpawnVolumeDatas.Contains(Id))
	{
		Data = SpawnVolumeDatas[Id];
	}
}

void UBOGameInstance::GetContainerData(FName Id, FSpawnStruct& Data)
{
	if (ContainerDatas.Contains(Id))
	{
		Data = ContainerDatas[Id];
	}
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

float UBOGameInstance::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UBOGameInstance::GetCurrentShield() const
{
	return CurrentShield;
}

int32 UBOGameInstance::GetTotalMoney() const
{
	return TotalMoney;
}
