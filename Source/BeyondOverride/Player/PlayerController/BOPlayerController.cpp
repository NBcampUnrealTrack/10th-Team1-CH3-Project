#include "Player/PlayerController/BOPlayerController.h"
#include "EnhancedInputSubsystems.h"

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
}

void ABOPlayerController::ShowMainHUDWidget()
{
}

void ABOPlayerController::ShowESCWidget()
{
}
