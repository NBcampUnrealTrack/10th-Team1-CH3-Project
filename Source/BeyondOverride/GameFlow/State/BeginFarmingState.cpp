// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/BeginFarmingState.h"

#include "../Manager/ContainerManager.h"
#include "../Manager/ExitManager.h"
#include "../Manager/SpawnVolumeManager.h"
#include "Kismet/GameplayStatics.h"

void UBeginFarmingState::Enter()
{
	SpawnCharacter();
	SpawnAI();
	ActivateContainer();
	ActivateExit();

	ChangeState(EFarmingState::Progress);
}

void UBeginFarmingState::SpawnCharacter()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerController || !Character || !GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		if (AActor* Exit = ExitManager->SelectRandomExit())
		{
			FVector ExitLocation = Exit->GetActorLocation();
			FRotator ExitRotation = Exit->GetActorRotation();

			Character->TeleportTo(ExitLocation, ExitRotation);
			PlayerController->SetControlRotation(ExitRotation);
		}
	}
}

void UBeginFarmingState::SpawnAI()
{
	if (USpawnVolumeManager* SpawnVolumeManager = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
	{
		SpawnVolumeManager->SpawnAI();
	}
}

void UBeginFarmingState::ActivateContainer()
{
	if (UContainerManager* ContainerManager = GetWorld()->GetGameInstance()->GetSubsystem<UContainerManager>())
	{
		ContainerManager->ActivateContainer();
	}
}

void UBeginFarmingState::ActivateExit()
{
	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->ActivateExit();
	}
}
