// Fill out your copyright notice in the Description page of Project Settings.

#include "BOGameMode.h"

#include "BOEnums.h"
#include "BOGameInstance.h"

#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"
#include "State/FarmingStateMachine.h"
#include "UI/Manager/UIManager.h"

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

		if (BOGameState == EGameState::Playing && BOPlayingState == EPlayingState::Farming)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Mode Begin Play"));
			StartFarming();
		}

		if (BOGameState == EGameState::Begin)
		{
			if (UUIManager* UIManager = UUIManager::Get(this))
			{
				UIManager->ShowScreen(EUIScreen::Title, EUIInputMode::UIOnly);
			}
		}
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
