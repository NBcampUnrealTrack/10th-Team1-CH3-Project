// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ExitManager.h"

#include "SpawnVolumeManager.h"

#include "Algo/RandomShuffle.h"
#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Spawn/SpawnVolume.h"
#include "Interaction/Actors/ExitControllerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/BOCharacter.h"
#include "Player/PlayerController/BOPlayerController.h"

void UExitManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GetWorld())
	{
		GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	}
}

void UExitManager::InitSetting()
{
	ExitControllers.Empty();

	if (!GetWorld())
	{
		return;
	}

	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	ExitActivateProb = DataAsset->GetExitActivateProb();

	TArray<AActor*> AllExitControllers{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitControllerActor::StaticClass(), AllExitControllers);

	for (AActor* Actor : AllExitControllers)
	{
		if (AExitControllerActor* ExitController = Cast<AExitControllerActor>(Actor))
		{
			ExitController->OnExtractControlRequested.AddDynamic(this, &UExitManager::HandleExtract);
			ExitControllers.Add(ExitController);
		}
	}

	SpawnCharacter();
	ActivateExit();
}

void UExitManager::SpawnCharacter()
{
	if (!GetWorld())
	{
		return;
	}

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

	if (AExitControllerActor* ExitController = SelectRandomExit())
	{
		if (AExitActor* Exit = ExitController->GetTargetExit())
		{
			ExitController->SetControllerAvailable(false);

			FVector ExitLocation = Exit->GetActorLocation();
			ExitLocation.Z += 100.0f;
			FRotator ExitRotation = Exit->GetActorRotation();

			Character->TeleportTo(ExitLocation, ExitRotation);
			PlayerController->SetControlRotation(ExitRotation);
		}
	}
}

void UExitManager::ActivateExit()
{
	int32 Size = ExitControllers.Num();
	int32 Count = FMath::RoundToInt(Size * ExitActivateProb);

	for (int i = 1; i <= Count; i++)
	{
		if (i < Size && ExitControllers[i])
		{
			ExitControllers[i]->SetControllerAvailable(true);
		}
	}
}

AExitControllerActor* UExitManager::SelectRandomExit()
{
	Algo::RandomShuffle(ExitControllers);

	if (ExitControllers.Num() != 0)
	{
		return ExitControllers[0];
	}
	else
	{
		return nullptr;
	}
}

void UExitManager::HandleExtract(AExitControllerActor* ExitPoint, AActor* Interactor)
{
	if (!ExitPoint || !GetWorld() || !GameInstance)
	{
		return;
	}

	USpawnVolumeManager* SpawnVolumeManager = GameInstance->GetSubsystem<USpawnVolumeManager>();
	if (!SpawnVolumeManager)
	{
		return;
	}

	FName RegionID = ExitPoint->GetRegionID();

	if (ASpawnVolume* SpawnVolume = SpawnVolumeManager->GetSpawnVolume(RegionID))
	{
		SpawnVolume->StartPhase();
	}
}

void UExitManager::CleanSetting()
{
	ExitControllers.Empty();
}
