#include "Player/PlayerController/BOPlayerController.h"

#include "EnhancedInputSubsystems.h"

#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"
#include "UI/Manager/UIManager.h"

ABOPlayerController::ABOPlayerController()
{
}

void ABOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	/*
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->BindInteractPrompt(InteractComponent);
	}

	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->ShowScreen(EUIScreen::HUD, EUIInputMode::GameOnly);
	}
	*/
}

void ABOPlayerController::ShowMainHUDWidget()
{
	if (UUIManager* UIManager = UUIManager::Get(this))
	{
		UIManager->ShowScreen(EUIScreen::HUD, EUIInputMode::GameOnly);
	}
}
