// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameInstance.h"

#include "BOGameMode.h"
#include "BOWorldSubsystem.h"

#include "Interaction/Actors/StorageContainerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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

	BODataAsset = nullptr;
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

	PlayerItemInventory.Empty();
	PlayerEquipmentInventory.Empty();
	StorageInventory.Empty();

	IsKeyCardAcquired = false;

	// MonsterDatas.Empty();
}

void UBOGameInstance::Start()
{
	GameState = EGameState::Playing;
	PlayingState = EPlayingState::Bunker;

	OpenLevel(ELevel::Bunker);
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
	PlayingState = EPlayingState::Farming;

	SurvivalTime = 0.0f;
	KilledMonsters.Empty();
	KillerMonster = "None";

	if (!IsKeyCardAcquired)
	{
		CheckKeyCard();
	}

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

	if (Level != ELevel::Bunker)
	{
		SaveStorageData();
	}

	if (GetWorld() && Levels.Contains(Level))
	{
		UGameplayStatics::OpenLevel(GetWorld(), Levels[Level]);
	}
}

void UBOGameInstance::SavePlayerData()
{
	UE_LOG(LogTemp, Warning, TEXT("Save Player Data"));

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
			CurShield = StatComponent->GetCurShield();
		}

		// inventory
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

void UBOGameInstance::CheckKeyCard()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UItemDataSubsystem* ItemDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemDataSubsystem>();
	if (!ItemDataSubsystem)
	{
		return;
	}

	for (TObjectPtr<UItemInstanceBase> Item : PlayerItemInventory)
	{
		if (IsValid(Item))
		{
			if (const FItemDataRow* ItemData = ItemDataSubsystem->GetItemData(Item->GetItemID()))
			{
				if (ItemData->DisplayName.EqualTo(FText::FromString(TEXT("KEY CARD"))))
				{
					IsKeyCardAcquired = true;

					return;
				}
			}
		}
	}

	for (TObjectPtr<UItemInstanceBase> Item : StorageInventory)
	{
		if (IsValid(Item))
		{
			if (const FItemDataRow* ItemData = ItemDataSubsystem->GetItemData(Item->GetItemID()))
			{
				if (ItemData->DisplayName.EqualTo(FText::FromString(TEXT("KEY CARD"))))
				{
					IsKeyCardAcquired = true;

					return;
				}
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

float UBOGameInstance::GetCurHealth() const
{
	return CurHealth;
}

float UBOGameInstance::GetCurShield() const
{
	return CurShield;
}

int32 UBOGameInstance::GetTotalMoney() const
{
	return TotalMoney;
}

bool UBOGameInstance::IsPlayerInventorySaved() const
{
	if (!PlayerItemInventory.IsEmpty() || !PlayerEquipmentInventory.IsEmpty())
	{
		return true;
	}

	return false;
}

bool UBOGameInstance::IsStorageInventorySaved() const
{
	if (!StorageInventory.IsEmpty())
	{
		return true;
	}

	return false;
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

bool UBOGameInstance::GetIsKeyCardAcquired() const
{
	return IsKeyCardAcquired;
}
