// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Spawn/SpawnVolume.h"

#include "../BOGameInstance.h"
#include "../Player/Character/BOCharacter.h"
#include "Kismet/GameplayStatics.h"

ASpawnVolume::ASpawnVolume()
	: Id("Default"),
	  SpawnExclusionRadius(0.0f)
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

	if (GetWorld())
	{
		if (UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>())
		{
			GameInstance->GetSpawnVolumeData(Id, SpawnVolumeData);
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

void ASpawnVolume::SpawnAI()
{
	int32 Count = SpawnVolumeData.SpawnCount;

	for (int i = 0; i < Count; i++)
	{
		SpawnRandomAI();
	}
}

void ASpawnVolume::SpawnRandomAI()
{
	if (!GetWorld() || !BoxComp)
	{
		return;
	}

	FVector SVLocation = GetActorLocation();
	FVector BoxExtent = BoxComp->GetScaledBoxExtent();
	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	float ExclusionDistance = FMath::Pow(SpawnExclusionRadius, 2);

	FVector SpawnLocation{};
	float X = FMath::RandRange(SVLocation.X - BoxExtent.X, SVLocation.X + BoxExtent.X);
	float Y = FMath::RandRange(SVLocation.Y - BoxExtent.Y, SVLocation.Y + BoxExtent.Y);
	float Distance = FMath::Pow(abs(PlayerLocation.X - X), 2) + FMath::Pow(abs(PlayerLocation.Y - Y), 2);

	// 최적화 생각하기
	while (Distance <= ExclusionDistance)
	{
		X = FMath::RandRange(SVLocation.X - BoxExtent.X, SVLocation.X + BoxExtent.X);
		Y = FMath::RandRange(SVLocation.Y - BoxExtent.Y, SVLocation.Y + BoxExtent.Y);
		Distance = FMath::Pow(abs(PlayerLocation.X - X), 2) + FMath::Pow(abs(PlayerLocation.Y - Y), 2);
	}

	SpawnLocation.X = X;
	SpawnLocation.Y = Y;
	SpawnLocation.Z = SVLocation.Z;

	TArray<FSpawnData> SpawnableAIs = SpawnVolumeData.SpawnableDatas;
	float Probability = FMath::RandRange(0.0f, 100.0f);
	float Sum{};

	for (FSpawnData SpawnableAI : SpawnableAIs)
	{
		Sum += SpawnableAI.Probability;

		if (Sum >= Probability)
		{
			// Get AI Data
			// Spawn AI
		}
	}
}
