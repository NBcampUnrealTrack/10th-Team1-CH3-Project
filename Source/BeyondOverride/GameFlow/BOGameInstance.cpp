// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameInstance.h"

#include "BOGameMode.h"
#include "BOWorldSubsystem.h"
#include "MoviePlayer.h"

#include "DataTables/Monster/MonsterInfo.h"
#include "Engine/AssetManager.h"
#include "GameFlow/Manager/LoadingScreenManager.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/BOLog.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/ActorComponent/StatComponent.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "Subsystems/ItemDataSubsystem.h"
#include "UI/Manager/UIManager.h"

void UBOGameInstance::Init()
{
	Super::Init();

	if (!GetWorld() || !BODataAsset)
	{
		return;
	}

	BODataAsset->GetLevels(Levels);

	LoadMonsterData();

	InitSetting();

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UBOGameInstance::OnPostLoadMap);
}

void UBOGameInstance::LoadMonsterData()
{
	if (!BODataAsset)
	{
		return;
	}

	if (UDataTable* MonsterData = BODataAsset->GetMonsterDataTable())
	{
		TArray<FMonsterInfo*> AllRows{};
		MonsterData->GetAllRows<FMonsterInfo>(TEXT("Get All Monster Datas"), AllRows);

		for (FMonsterInfo* Row : AllRows)
		{
			if (Row)
			{
				FName Id = Row->MonsterID;
				MonsterDatas.Add(Id, *Row);
			}
		}
	}
}

void UBOGameInstance::InitSetting()
{
	GameState = EGameState::Begin;
	PlayingState = EPlayingState::None;
	DeathLocation = EDeathLocation::None;
	FarmingResult = EFarmingResult::None;
	CurLevel = ELevel::Basic;

	TotalSurvivalTime = 0.0f;
	SurvivalTime = 0.0f;
	TotalKilledMonsters.Empty();
	KilledMonsters.Empty();
	KillerMonster = "None";

	CurHealth = 0;
	MaxHealth = 0;
	CurShield = 0;
	MaxShield = 0;

	PlayerItemInventory.Empty();
	PlayerEquipmentInventory.Empty();
	StorageInventory.Empty();
	MonsterDatas.Empty();

	OpenLevel(CurLevel);
}

void UBOGameInstance::Start()
{
	GameState = EGameState::Playing;
	PlayingState = EPlayingState::Bunker;

	OpenLevel(ELevel::Bunker);
}

void UBOGameInstance::End()
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
	PlayingState = EPlayingState::Farming;
	FarmingResult = EFarmingResult::None;

	SurvivalTime = 0.0f;
	KilledMonsters.Empty();
	KillerMonster = "None";

	OpenLevel(ELevel::Main);
}

void UBOGameInstance::EndFarming(EFarmingResult Result)
{
	PlayingState = EPlayingState::Bunker;
	FarmingResult = Result;
	FarmingCount += 1;

	if (Result == EFarmingResult::Fail)
	{
		DeathLocation = EDeathLocation::Main;
		DeathCount += 1;
	}

	SaveFarmingData();

	if (Result != EFarmingResult::Clear)
	{
		OpenLevel(ELevel::Bunker);
	}
}

void UBOGameInstance::Die()
{
	if (PlayingState == EPlayingState::Bunker)
	{
		DeathLocation = EDeathLocation::Bunker;

		OpenLevel(ELevel::Bunker);
	}
	else if (PlayingState == EPlayingState::Farming)
	{
		DeathLocation = EDeathLocation::Main;
	}
}

void UBOGameInstance::EnterServerRoom()
{
	SavePlayerData();
	SaveFarmingData();

	OpenLevel(ELevel::ServerRoom);
}

void UBOGameInstance::OpenLevel(ELevel Level)
{
	CurLevel = Level;

	SavePlayerData();

	if (PlayingState == EPlayingState::Bunker)
	{
		SaveStorageData();
	}

	if (!Levels.Contains(Level))
	{
		return;
	}

	if (Level != ELevel::Basic)
	{
		ShowLoadingScreenWidget(true);
	}

	if (GetWorld())
	{
		UGameplayStatics::OpenLevel(GetWorld(), Levels[Level]);
	}
}

void UBOGameInstance::ShowLoadingScreenWidget(bool IsNew)
{
	if (ULoadingScreenManager* LoadingScreenManager = GetSubsystem<ULoadingScreenManager>())
	{
		LoadingScreenManager->ShowLoadingScreenWidget(IsNew);
	}
}

void UBOGameInstance::OnPostLoadMap(UWorld* World)
{
	if (CurLevel == ELevel::Basic)
	{
		return;
	}

	ShowLoadingScreenWidget(false);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HideLoadingScreenTimer);
		GetWorld()->GetTimerManager().SetTimer(HideLoadingScreenTimer, this, &UBOGameInstance::HideLoadingScreenWidget, 5.0f, false);
	}
}

void UBOGameInstance::HideLoadingScreenWidget()
{
	if (ULoadingScreenManager* LoadingScreenManager = GetSubsystem<ULoadingScreenManager>())
	{
		LoadingScreenManager->HideLoadingScreenWidget();
	}

	OnLevelPrepared();
}

void UBOGameInstance::OnCharacterPrepared()
{
	if (CurLevel == ELevel::Basic)
	{
		OnLevelPrepared();
	}
}

void UBOGameInstance::OnLevelPrepared()
{
	if (ABOGameMode* GameMode = GetWorld()->GetAuthGameMode<ABOGameMode>())
	{
		GameMode->InitSetting();
	}
}

void UBOGameInstance::SavePlayerData()
{
	PlayerItemInventory.Empty();
	PlayerEquipmentInventory.Empty();

	if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	if (ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>())
	{
		// health, shield
		if (UStatComponent* StatComponent = Character->GetStatComponent())
		{
			CurHealth = StatComponent->GetCurHealth();
			MaxHealth = StatComponent->GetMaxHealth();
			CurShield = StatComponent->GetCurShield();
			MaxShield = StatComponent->GetMaxShield();
		}

		// inventory
		if (FarmingResult != EFarmingResult::Fail)
		{
			if (UPlayerInventoryComponent* InventoryComponent = Character->GetPlayerInventoryComponent())
			{
				TArray<UItemInstanceBase*> InventorySlots = InventoryComponent->GetSlots();
				TArray<UItemInstanceBase*> EquipmentSlots = InventoryComponent->GetEquipmentSlots();

				for (UItemInstanceBase* InventorySlot : InventorySlots)
				{
					UItemInstanceBase* Item = DuplicateObject<UItemInstanceBase>(InventorySlot, this);
					PlayerItemInventory.Add(Item);
				}

				for (UItemInstanceBase* EquipmentSlot : EquipmentSlots)
				{
					UItemInstanceBase* Item = DuplicateObject<UItemInstanceBase>(EquipmentSlot, this);
					PlayerEquipmentInventory.Add(Item);
				}
			}
		}
	}
}

void UBOGameInstance::SaveStorageData()
{
	StorageInventory.Empty();

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageContainerActor::StaticClass(), AllActors);

	if (!AllActors.IsEmpty())
	{
		if (AStorageContainerActor* Storage = Cast<AStorageContainerActor>(AllActors[0]))
		{
			if (UInventoryComponent* InventoryComponent = Storage->GetInventoryComponent())
			{
				TArray<UItemInstanceBase*> Slots = InventoryComponent->GetSlots();

				for (UItemInstanceBase* Slot : Slots)
				{
					UItemInstanceBase* Item = DuplicateObject<UItemInstanceBase>(Slot, this);
					StorageInventory.Add(Item);
				}
			}
		}
	}
}

void UBOGameInstance::SaveFarmingData()
{
	SaveSurvivalTimeData();
	SaveCombatData();
}

void UBOGameInstance::SaveSurvivalTimeData()
{
	if (!GetWorld())
	{
		return;
	}

	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		SurvivalTime = WorldSubsystem->GetSurvivalTime();

		TotalSurvivalTime += SurvivalTime;

		UE_LOG(LogGameFlow, Warning, TEXT("Survival Time : %f"), SurvivalTime);
		UE_LOG(LogGameFlow, Warning, TEXT("Total Survival Time : %f"), TotalSurvivalTime);
	}
}

void UBOGameInstance::SaveCombatData()
{
	if (!GetWorld())
	{
		return;
	}

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

void UBOGameInstance::GetMonsterData(FName Id, FMonsterInfo& Data) const
{
	if (MonsterDatas.Contains(Id))
	{
		Data = MonsterDatas[Id];
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

EDeathLocation UBOGameInstance::GetDeathLocation() const
{
	return DeathLocation;
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

int32 UBOGameInstance::GetFarmingCount() const
{
	return FarmingCount;
}

int32 UBOGameInstance::GetDeathCount() const
{
	return DeathCount;
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

float UBOGameInstance::GetCurHealth() const
{
	return CurHealth;
}

float UBOGameInstance::GetMaxHealth() const
{
	return MaxHealth;
}

float UBOGameInstance::GetCurShield() const
{
	return CurShield;
}

float UBOGameInstance::GetMaxShield() const
{
	return MaxShield;
}

bool UBOGameInstance::GetIsLevelPreparing() const
{
	return IsLevelPreparing;
}

TArray<UItemInstanceBase*> UBOGameInstance::GetPlayerItemInventory() const
{
	return PlayerItemInventory;
}

TArray<UItemInstanceBase*> UBOGameInstance::GetPlayerEquipmentInventory() const
{
	return PlayerEquipmentInventory;
}

TArray<UItemInstanceBase*> UBOGameInstance::GetStorageInventory() const
{
	return StorageInventory;
}

TMap<FName, FMonsterInfo> UBOGameInstance::GetMonsterDatas() const
{
	return MonsterDatas;
}
