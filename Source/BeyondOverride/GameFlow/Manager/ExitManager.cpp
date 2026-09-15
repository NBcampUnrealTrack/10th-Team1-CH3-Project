// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ExitManager.h"

#include "SpawnVolumeManager.h"

#include "Algo/RandomShuffle.h"
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
		if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
		{
			ExitActivateProb = GameInstance->GetExitActivateProb();
		}
	}
}

void UExitManager::InitSetting()
{
	Exits.Empty();
	ExitControllers.Empty();

	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllExits{};
	TArray<AActor*> AllExitControllers{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitActor::StaticClass(), AllExits);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitControllerActor::StaticClass(), AllExitControllers);

	for (AActor* Actor : AllExits)
	{
		if (AExitActor* Exit = Cast<AExitActor>(Actor))
		{
			Exits.Add(Exit);
		}
	}

	for (AActor* Actor : AllExitControllers)
	{
		if (AExitControllerActor* ExitController = Cast<AExitControllerActor>(Actor))
		{
			// ExitController->OnExtractRequested.AddDynamic(this, &UExitManager::HandleExtract);
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

	if (AExitActor* Exit = SelectRandomExit())
	{
		Exit->SetExtractAvailable(false);

		FVector ExitLocation = Exit->GetActorLocation();
		ExitLocation.Z += 100.0f;
		FRotator ExitRotation = Exit->GetActorRotation();

		Character->TeleportTo(ExitLocation, ExitRotation);
		PlayerController->SetControlRotation(ExitRotation);
	}
}

void UExitManager::ActivateExit()
{
	int32 Size = Exits.Num();
	int32 Count = FMath::RoundToInt(Size * ExitActivateProb) - 1; // except character spawn point

	Algo::RandomShuffle(Exits);

	for (int i = 0; i < Count; i++)
	{
		if (Exits[i])
		{
			// Exits[i]->SetExtractAvailable(true);
		}
	}
}

AExitActor* UExitManager::SelectRandomExit()
{
	int32 Size = Exits.Num();
	int32 Index = FMath::RandRange(0, Size - 1);

	if (Index < Size)
	{
		return Exits[Index];
	}
	else
	{
		return nullptr;
	}
}

void UExitManager::HandleExtract(AExitActor* ExitPoint, AActor* Interactor)
{
	if (!ExitPoint || !GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	USpawnVolumeManager* SpawnVolumeManager = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>();
	if (!SpawnVolumeManager)
	{
		return;
	}

	FName RegionId = ExitPoint->RegionId; // change to getter function

	if (ASpawnVolume* SpawnVolume = SpawnVolumeManager->GetSpawnVolume(RegionId))
	{
		SpawnVolume->StartPhase();
	}
}
