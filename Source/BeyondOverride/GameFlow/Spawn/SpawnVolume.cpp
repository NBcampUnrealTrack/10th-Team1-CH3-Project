// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Spawn/SpawnVolume.h"

#include "../Manager/SpawnVolumeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/BOCharacter.h"

ASpawnVolume::ASpawnVolume()
	: PhaseIndex(0)
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collsion"));
	BoxComp->SetupAttachment(RootComponent);
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		if (USpawnVolumeManager* SpawnVolumeManager = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
		{
			SpawnVolumeManager->GetSpawnVolumeData(Id, SpawnVolumeData);
			SpawnVolumeManager->GetPhaseData(Id, PhaseData);
		}
	}

	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpawnVolume::OnOverlapped);
		BoxComp->SetGenerateOverlapEvents(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Box Component"));
	}
}

void ASpawnVolume::OnOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Volume Overlapped"));
	UE_LOG(LogTemp, Warning, TEXT("Overlap Actor : %s"), *OtherActor->GetName());

	if (OtherActor->IsA<ABOCharacter>())
	{
		OnPlayerEntered.ExecuteIfBound(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Player"));
	}
}

void ASpawnVolume::SpawnMonsters()
{
	int32 Count = FMath::RandRange(SpawnVolumeData.MinSpawnCount, SpawnVolumeData.MaxSpawnCount);
	TArray<FSpawnEntry> SpawnEntries = SpawnVolumeData.SpawnEntries;

	UE_LOG(LogTemp, Warning, TEXT("Spawn Volume : %s"), *Id.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Count : %d"), Count);
	for (int i = 0; i < Count; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Random Monster"));
		SpawnRandomMonster(SpawnEntries, SpawnMinRadius);
	}
}

void ASpawnVolume::SpawnRandomMonster(TArray<FSpawnEntry>& SpawnEntries, float MinDist, float MaxDist, bool IsChase)
{
	if (!GetWorld() || !BoxComp)
	{
		return;
	}

	float MinDistance = FMath::Pow(SpawnMinRadius, 2);
	float MaxDistance = FMath::Pow(SpawnMaxRadius, 2);

	if (MinDist != -1.0f)
	{
		MinDistance = FMath::Pow(MinDist, 2);
	}
	if (MaxDist != -1.0f)
	{
		MaxDistance = FMath::Pow(MaxDist, 2);
	}

	FVector SVLocation = GetActorLocation();
	FVector BoxExtent = BoxComp->GetScaledBoxExtent();
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	FVector SpawnLocation{};
	float X = FMath::RandRange(SVLocation.X - BoxExtent.X, SVLocation.X + BoxExtent.X);
	float Y = FMath::RandRange(SVLocation.Y - BoxExtent.Y, SVLocation.Y + BoxExtent.Y);
	float Distance = FMath::Pow(abs(PlayerLocation.X - X), 2) + FMath::Pow(abs(PlayerLocation.Y - Y), 2);

	// Is it Optimal?
	while (Distance < MinDistance || MaxDistance < Distance)
	{
		X = FMath::RandRange(SVLocation.X - BoxExtent.X, SVLocation.X + BoxExtent.X);
		Y = FMath::RandRange(SVLocation.Y - BoxExtent.Y, SVLocation.Y + BoxExtent.Y);
		Distance = FMath::Pow(abs(PlayerLocation.X - X), 2) + FMath::Pow(abs(PlayerLocation.Y - Y), 2);
	}

	SpawnLocation.X = X;
	SpawnLocation.Y = Y;
	SpawnLocation.Z = SVLocation.Z;

	float Prob = FMath::RandRange(0.0f, 100.0f);
	float Sum{};

	for (FSpawnEntry SpawnEntry : SpawnEntries)
	{
		Sum += SpawnEntry.Prob;

		if (Sum >= Prob)
		{
			FName MonsterId = SpawnEntry.Id;
			UE_LOG(LogTemp, Warning, TEXT("Spawned Monster : %s"), *MonsterId.ToString());

			// Get AI Data
			// Spawn AI

			if (IsChase)
			{
				// Chase Player
			}
		}
	}
}

void ASpawnVolume::StartPhase()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(PhaseTimer);

	int32 Size = PhaseData.PhaseEntries.Num();

	if (Size == 0)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Start Phase"));
	UE_LOG(LogTemp, Warning, TEXT("Phase Count : %d"), Size);

	SpawnPhaseMonsters();
}

void ASpawnVolume::SpawnPhaseMonsters()
{
	int32 Size = PhaseData.PhaseEntries.Num();

	UE_LOG(LogTemp, Warning, TEXT("Spawn Phase Monster"));

	TArray<FPhaseEntry> PhaseEntries = PhaseData.PhaseEntries;
	TArray<FSpawnEntry> SpawnEntries = PhaseEntries[PhaseIndex].SpawnEntries;
	int32 SpawnCount = PhaseEntries[PhaseIndex].SpawnCount;

	UE_LOG(LogTemp, Warning, TEXT("Phase Monster Count : %d"), SpawnCount);

	for (int i = 0; i < SpawnCount; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Phase Random Monster"));
		SpawnRandomMonster(SpawnEntries, SpawnMinRadius, SpawnMaxRadius, true);
	}

	if (PhaseIndex == Size - 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("End Phase"));
		PhaseIndex = 0;

		return;
	}

	float Duration = PhaseData.PhaseEntries[PhaseIndex].Duration;

	GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &ASpawnVolume::StartPhase, Duration, false);

	PhaseIndex += 1;
}

FName ASpawnVolume::GetId() const
{
	return Id;
}

FName ASpawnVolume::GetRegionId() const
{
	return SpawnVolumeData.RegionId;
}

void ASpawnVolume::CleanSetting()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTimer);
	}
}
