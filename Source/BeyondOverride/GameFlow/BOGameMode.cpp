// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameMode.h"

#include "BOGameInstance.h"

#include "DataTables/Items/ItemDataRow.h"
#include "Engine/TargetPoint.h"
#include "Factory/ItemFactory.h"
#include "Interaction/Actors/StorageContainerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ActorComponent/InventoryComponent.h"
#include "Player/ActorComponent/PlayerInventoryComponent.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "State/FarmingStateMachine.h"
#include "UI/Manager/UIManager.h"

ABOGameMode::ABOGameMode()
	: StateMachine(nullptr)
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

	UE_LOG(LogTemp, Warning, TEXT("Game Mode BeginPlay"));

	if (GameInstance)
	{
		EGameState BOGameState = GameInstance->GetGameState();
		EPlayingState BOPlayingState = GameInstance->GetPlayingState();
		EFarmingResult BOFarmingResult = GameInstance->GetFarmingResult();

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
				EnterBunker(BOFarmingResult);
			}
			else if (BOPlayingState == EPlayingState::Farming)
			{
				StartFarming();
			}
		}
	}
}

void ABOGameMode::Start()
{
	if (GameInstance)
	{
		GameInstance->Start();
	}
}

void ABOGameMode::EnterBunker(EFarmingResult Result)
{
	if (Result != EFarmingResult::Success)
	{
		ProvideBasicEquipment();
	}

	if (Result != EFarmingResult::None)
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

	TArray<FName> BasicEquipments{};
	GameInstance->GetBasicEquipments(BasicEquipments);

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
			ItemFactory.SpawnItemPickup(GetWorld(), BasicEquipments[Tag], 1, Location, Rotation);
		}
	}
}

void ABOGameMode::StartFarming()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Mode Begin Farming"));
	StateMachine = NewObject<UFarmingStateMachine>(this, UFarmingStateMachine::StaticClass());

	if (StateMachine)
	{
		StateMachine->Initialize(this);
		StateMachine->ChangeState(EFarmingState::Begin);
	}
}

void ABOGameMode::EndFarming(EFarmingResult Result)
{
	UE_LOG(LogTemp, Warning, TEXT("Game Mode End Farming"));
	if (StateMachine)
	{
		StateMachine->SetFarmingResult(Result);
		StateMachine->ChangeState(EFarmingState::End);
	}
}

void ABOGameMode::ToEnding()
{
	if (GameInstance)
	{
		GameInstance->ToEnding();
	}
}

void ABOGameMode::Explosion()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->ShowScreen(EUIScreen::FinalResult, EUIInputMode::UIOnly);
	}
}

void ABOGameMode::End()
{
	if (GameInstance)
	{
		GameInstance->End();
	}

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->ShowScreen(EUIScreen::Title, EUIInputMode::UIOnly);
	}
}

void ABOGameMode::Exit()
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
