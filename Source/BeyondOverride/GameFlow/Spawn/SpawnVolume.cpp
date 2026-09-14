// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Spawn/SpawnVolume.h"

#include "../Manager/SpawnVolumeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Character/BOCharacter.h"

ASpawnVolume::ASpawnVolume()
	: Id("Default"),
	  SpawnMinRadius(0.0f),
	  PhaseIndex(0)
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collsion"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawnVolume::OnOverlapped);
	BoxComp->SetGenerateOverlapEvents(true);
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
}

void ASpawnVolume::OnOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABOCharacter>())
	{
		OnPlayerEntered.ExecuteIfBound(this);
	}
}

void ASpawnVolume::SpawnMonsters()
{
	int32 Count = FMath::RandRange(SpawnVolumeData.MinSpawnCount, SpawnVolumeData.MaxSpawnCount);
	TArray<FSpawnEntry> SpawnEntries = SpawnVolumeData.SpawnEntries;

	for (int i = 0; i < Count; i++)
	{
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
	float MaxDistance{};

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

	if (PhaseIndex == Size)
	{
		PhaseIndex = 0;

		return;
	}

	float Duration = PhaseData.PhaseEntries[PhaseIndex].Duration;

	GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &ASpawnVolume::StartPhase, Duration, false);
	SpawnPhaseMonsters();
}

void ASpawnVolume::SpawnPhaseMonsters()
{
	TArray<FPhaseEntry> PhaseEntries = PhaseData.PhaseEntries;
	TArray<FSpawnEntry> SpawnEntries = PhaseEntries[PhaseIndex].SpawnEntries;
	int32 SpawnCount = PhaseEntries[PhaseIndex].SpawnCount;

	for (int i = 0; i < SpawnCount; i++)
	{
		SpawnRandomMonster(SpawnEntries, SpawnMinRadius, SpawnMaxRadius, true);
	}

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
