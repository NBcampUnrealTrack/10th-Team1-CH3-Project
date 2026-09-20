// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameMode.h"

#include "BOGameInstance.h"

#include "DataAssets/BODataAsset.h"
#include "DataTables/Items/ItemDataRow.h"
#include "Engine/TargetPoint.h"
#include "Factory/ItemFactory.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Items/Actors/ItemPickupBase.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "State/FarmingStateMachine.h"
#include "UI/Manager/UIManager.h"

ABOGameMode::ABOGameMode()
	: FarmingStateMachine(nullptr)
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	GameInstance = GetGameInstance<UBOGameInstance>();

	PlayerControllerClass = ABOPlayerController::StaticClass();
	DefaultPawnClass = ABOCharacter::StaticClass();
}

void ABOGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogGameFlow, Warning, TEXT("Game Mode BeginPlay"));
}

void ABOGameMode::InitSetting()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Game Mode Initial Setting"));

	if (GameInstance)
	{
		EGameState BOGameState = GameInstance->GetGameState();
		EPlayingState BOPlayingState = GameInstance->GetPlayingState();

		if (BOGameState == EGameState::Begin)
		{
			if (UUIManager* UIManager = UUIManager::Get(this))
			{
				UIManager->ShowScreen(EUIScreen::Title, EUIInputMode::UIOnly);
			}
		}
		else if (BOGameState == EGameState::Playing)
		{
			if (UUIManager* UIManager = UUIManager::Get(this))
			{
				UIManager->ShowScreen(EUIScreen::HUD, EUIInputMode::GameOnly);
			}

			if (BOPlayingState == EPlayingState::Bunker)
			{
				EnterBunker();
			}
			else if (BOPlayingState == EPlayingState::Farming)
			{
				StartFarming();
			}
		}
	}
}

void ABOGameMode::StartGame()
{
	if (GameInstance)
	{
		GameInstance->Start();
	}
}

void ABOGameMode::EnterBunker()
{
	if (!GameInstance)
	{
		return;
	}

	EFarmingResult FarmingResult = GameInstance->GetFarmingResult();
	EDeathLocation DeathLocation = GameInstance->GetDeathLocation();

	if (DeathLocation != EDeathLocation::Bunker && FarmingResult != EFarmingResult::Success)
	{
		ProvideBasicEquipment();
	}

	if (FarmingResult != EFarmingResult::None)
	{
		if (UUIManager* UIManager = UUIManager::Get(this))
		{
			UIManager->PushScreen(EUIScreen::Result, EUIInputMode::UIOnly);
		}
	}
}

void ABOGameMode::ProvideBasicEquipment()
{
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	TArray<FName> BasicEquipments{};
	DataAsset->GetBasicEquipments(BasicEquipments);

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), AllActors);

	int32 Count = AllActors.Num();

	FItemFactory ItemFactory{};

	for (int i = 0; i < Count; i++)
	{
		AActor* TargetPoint = AllActors[i];
		if (TargetPoint)
		{
			int32 Tag = FCString::Atoi(*TargetPoint->Tags[0].ToString());
			FVector Location = TargetPoint->GetActorLocation();
			FRotator Rotation = TargetPoint->GetActorRotation();
			ItemFactory.SpawnItemPickup(GetWorld(), BasicEquipments[Tag], 0, Location, Rotation);
		}
	}
}

void ABOGameMode::StartFarming()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Game Mode Begin Farming"));
	FarmingStateMachine = NewObject<UFarmingStateMachine>(this, UFarmingStateMachine::StaticClass());

	if (FarmingStateMachine)
	{
		FarmingStateMachine->Initialize(this);
		FarmingStateMachine->ChangeState(EFarmingState::Begin);
	}
}

void ABOGameMode::EndFarming(EFarmingResult Result)
{
	UE_LOG(LogGameFlow, Warning, TEXT("Game Mode End Farming"));
	if (FarmingStateMachine)
	{
		FarmingStateMachine->SetFarmingResult(Result);
		FarmingStateMachine->ChangeState(EFarmingState::End);
	}
}

void ABOGameMode::Die()
{
	if (!GameInstance)
	{
		return;
	}

	GameInstance->Die();

	EPlayingState PlayingState = GameInstance->GetPlayingState();

	if (PlayingState == EPlayingState::Farming)
	{
		EndFarming(EFarmingResult::Fail);
	}
}

void ABOGameMode::EnterServerRoom()
{
	if (GameInstance)
	{
		GameInstance->EnterServerRoom();
	}
}

void ABOGameMode::StartBossBattle()
{
	// request to monster spawn system
	// spawn boss
	// start boss phase
}

void ABOGameMode::StartDefense()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Game Mode Begin Defense"));
	/*DefenseStateMachine = NewObject<UDefenseStateMachine>(this, UDefenseStateMachine::StaticClass());

	if (DefenseStateMachine)
	{
		DefenseStateMachine->Initialize(this);
		DefenseStateMachine->ChangeState(EFarmingState::Begin);
	}*/
}

void ABOGameMode::ClearGame()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->ShowScreen(EUIScreen::FinalResult, EUIInputMode::UIOnly);
	}
}

void ABOGameMode::Ending()
{
	if (GameInstance)
	{
		GameInstance->End();
	}
}

void ABOGameMode::ExitGame()
{
	if (GameInstance)
	{
		GameInstance->Exit();
	}
}

void ABOGameMode::AddKilledMonster(FName MonsterId)
{
	if (KilledMonsters.Contains(MonsterId))
	{
		KilledMonsters[MonsterId] += 1;
	}
	else
	{
		KilledMonsters.Add(MonsterId, 1);
	}
}

void ABOGameMode::SetKillerMonster(FName MonsterId)
{
	KillerMonster = MonsterId;
}

void ABOGameMode::GetKilledMonsters(TMap<FName, int32>& Data) const
{
	Data = KilledMonsters;
}

FName ABOGameMode::GetKillerMonster() const
{
	return KillerMonster;
}
