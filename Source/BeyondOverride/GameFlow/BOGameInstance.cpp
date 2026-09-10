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
	// Inventory.empty();

	OpenLevel(ELevel::Bunker);
	StartFarming();
}

void UBOGameInstance::LoadSpawnVolumeData()
{
}

void UBOGameInstance::Start()
{
	GameState = EGameState::Playing;
	PlayingState = EPlayingState::Shelter;

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
	GameState = EGameState::Playing;  // test code
	PlayingState = EPlayingState::Farming;

	OpenLevel(ELevel::Main);
}

void UBOGameInstance::EndFarming(EFarmingResult Result)
{
	PlayingState = EPlayingState::Shelter;
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

void UBOGameInstance::GetSpawnVolumeData(FName Id, FSpawnVolumeData& Data)
{
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
