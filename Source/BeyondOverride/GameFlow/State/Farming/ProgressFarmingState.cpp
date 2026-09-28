// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Farming/ProgressFarmingState.h"

#include "Engine/TargetPoint.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOWorldSubsystem.h"
#include "GameFlow/Manager/ExitManager.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"

void UProgressFarmingState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Farming Progress Enter"));

	Super::Enter();

	SpawnCharacter();
	ActivateExits();
	// SetStartTime();
}

void UProgressFarmingState::SpawnCharacter()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	ELevel PrevLevel = GameInstance->GetPrevLevel();
	ELevel CurLevel = GameInstance->GetCurLevel();

	if (PrevLevel == ELevel::AIBuilding && CurLevel == ELevel::Main)
	{
		TeleportCharacter();

		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->SpawnCharacter();
	}
}

void UProgressFarmingState::TeleportCharacter()
{
	ABOPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ABOPlayerController>();
	if (!PlayerController)
	{
		return;
	}

	ABOCharacter* Character = PlayerController->GetPawn<ABOCharacter>();
	if (!Character)
	{
		return;
	}

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (!Actor)
		{
			continue;
		}

		ATargetPoint* TargetPoint = Cast<ATargetPoint>(Actor);
		if (!TargetPoint)
		{
			continue;
		}

		if (TargetPoint->ActorHasTag(FName(TEXT("AIBuildingEntrance"))))
		{
			FVector Location = TargetPoint->GetActorLocation();
			FRotator Rotation = TargetPoint->GetActorRotation();
			Rotation.Yaw += 180.0f;

			Character->TeleportTo(Location, Rotation);
			PlayerController->SetControlRotation(Rotation);
		}
	}
}

void UProgressFarmingState::ActivateExits()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	if (UExitManager* ExitManager = GetWorld()->GetGameInstance()->GetSubsystem<UExitManager>())
	{
		ExitManager->ActivateExits();
	}
}

void UProgressFarmingState::SetStartTime()
{
	if (!GetWorld())
	{
		return;
	}

	if (UBOWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UBOWorldSubsystem>())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Set Start Time"));
		WorldSubsystem->SetStartTime();
	}
}
