// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameMode.h"

#include "BOGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "State/FarmingStateMachine.h"

ABOGameMode::ABOGameMode()
	: StateMachine(nullptr)
{
	PlayerControllerClass = ABOPlayerController::StaticClass();
	DefaultPawnClass = ABOCharacter::StaticClass();
}

void ABOGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UBOGameInstance* GameInstance = GetGameInstance<UBOGameInstance>())
	{
		EGameState BOGameState = GameInstance->GetGameState();
		EPlayingState BOPlayingState = GameInstance->GetPlayingState();
		EFarmingResult BOFarmingResult = GameInstance->GetFarmingResult();

		if (BOGameState == EGameState::Playing)
		{
			if (BOPlayingState == EPlayingState::Bunker && BOFarmingResult != EFarmingResult::Success)
			{
				UE_LOG(LogTemp, Warning, TEXT("Provide Basic Equipments"));
				ProvideBasicEquipment();
			}
			else if (BOPlayingState == EPlayingState::Farming)
			{
				UE_LOG(LogTemp, Warning, TEXT("Game Mode Begin Play"));
				StartFarming();
			}
		}
	}
}

void ABOGameMode::ProvideBasicEquipment()
{
	if (!GetWorld())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	TArray<FName> BasicEquipments{};
	GameInstance->GetBasicEquipments(BasicEquipments);

	TArray<AActor*> AllActors{};
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors); // change AActor -> AContainer
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Container"), AllActors); // test code

	if (!AllActors.IsEmpty())
	{
		// provide basic equipments to bunker container
		// AllActors[0]->SetInventoryItems(BasicEquipments);
	}
}

void ABOGameMode::StartFarming()
{
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

void ABOGameMode::GetKilledMonsters(TMap<FName, int32>& Data) const
{
	Data = KilledMonsters;
}

FName ABOGameMode::GetKillerMonster() const
{
	return KillerMonster;
}
