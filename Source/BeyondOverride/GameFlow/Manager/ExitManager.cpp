// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Manager/ExitManager.h"

#include "SpawnVolumeManager.h"

#include "../../Interaction/Actors/ExitActor.h"
#include "../BOGameInstance.h"
#include "../Spawn/SpawnVolume.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"

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

	if (!GetWorld())
	{
		return;
	}

	TArray<AActor*> AllActors{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		if (AExitActor* Exit = Cast<AExitActor>(Actor))
		{
			Exit->OnExtractRequested.AddDynamic(this, &UExitManager::HandleExtract);
			Exits.Add(Exit);
		}
	}

	SpawnCharacter();
	ActivateExit();
}

void UExitManager::SpawnCharacter()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerController || !Character)
	{
		return;
	}

	if (AExitActor* Exit = SelectRandomExit())
	{
		Exit->SetExtractAvailable(false);

		FVector ExitLocation = Exit->GetActorLocation();
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
			Exits[i]->SetExtractAvailable(true);
		}
	}
}

AExitActor* UExitManager::SelectRandomExit()
{
	int32 Size = Exits.Num();
	int32 Index = FMath::RandRange(0, Size - 1);

	return Exits[Index];
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

	/*FName RegionId = ExitPoint->GetRegionId();

	if (ASpawnVolume* SpawnVolume = SpawnVolumeManager->GetSpawnVolume(RegionId))
	{
		SpawnVolume->StartPhase();
	}*/
}
