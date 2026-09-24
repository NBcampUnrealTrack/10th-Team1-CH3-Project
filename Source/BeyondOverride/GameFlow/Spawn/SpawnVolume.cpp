// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/Spawn/SpawnVolume.h"

#include "DataTables/Monster/MonsterInfo.h"
#include "Engine/EngineTypes.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/Manager/ContainerManager.h"
#include "GameFlow/Manager/SpawnVolumeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/MonsterCalling.h"
#include "Monster/System/MonsterSpawn.h"
#include "Player/Character/BOCharacter.h"

ASpawnVolume::ASpawnVolume()
	: PhaseIndex(0)
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collsion"));
	BoxComp->SetupAttachment(RootComponent);

	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13); // SpawnVolume
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel14, ECollisionResponse::ECR_Overlap); // Container
}

void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13); // SpawnVolume
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel14, ECollisionResponse::ECR_Overlap); // Container

	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		if (USpawnVolumeManager* SpawnVolumeManager = GetWorld()->GetGameInstance()->GetSubsystem<USpawnVolumeManager>())
		{
			SpawnVolumeManager->GetSpawnVolumeData(RegionID, SpawnVolumeData);
			SpawnVolumeManager->GetPhaseData(RegionID, PhaseData);
		}
	}

	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpawnVolume::OnOverlapped);
		BoxComp->SetGenerateOverlapEvents(true);
	}
}

void ASpawnVolume::OnOverlapped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogGameFlow, Warning, TEXT("%s Overlapped"), *RegionID.ToString());
	UE_LOG(LogGameFlow, Warning, TEXT("Overlap Actor : %s"), *OtherActor->GetName());

	if (OtherActor->IsA<ABOCharacter>())
	{
		UE_LOG(LogGarbage, Warning, TEXT("Player Overlapped %s"), *RegionID.ToString());

		if (BoxComp)
		{
			UE_LOG(LogGameFlow, Warning, TEXT("%s Remove Overlap Bind"), *RegionID.ToString());

			BoxComp->OnComponentBeginOverlap.RemoveDynamic(this, &ASpawnVolume::OnOverlapped);
			BoxComp->SetGenerateOverlapEvents(false);

			SpawnMonsters();
			ActivateContainers();
		}
	}
	else
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Not Player"));
	}
}

void ASpawnVolume::SpawnMonsters()
{
	int32 Count = FMath::RandRange(SpawnVolumeData.MinSpawnCount, SpawnVolumeData.MaxSpawnCount);
	TArray<FSpawnEntry> SpawnEntries = SpawnVolumeData.SpawnEntries;

	UE_LOG(LogGameFlow, Warning, TEXT("Spawn Volume : %s"), *RegionID.ToString());
	UE_LOG(LogGameFlow, Warning, TEXT("Count : %d"), Count);

	for (int i = 0; i < Count; i++)
	{
		SpawnRandomMonster(SpawnEntries, SpawnMinRadius);
	}
}

void ASpawnVolume::SpawnRandomMonster(TArray<FSpawnEntry>& SpawnEntries, float MinDist, float MaxDist, bool IsChase)
{
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !BoxComp)
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UMonsterSpawn* MonsterSpawnSystem = NewObject<UMonsterSpawn>(this);
	if (!MonsterSpawnSystem)
	{
		return;
	}

	UMonsterCalling* MonsterCallingSystem = NewObject<UMonsterCalling>(this);
	if (!MonsterCallingSystem)
	{
		return;
	}

	ABOCharacter* Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>();
	if (!Character)
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
	FVector PlayerLocation = Character->GetActorLocation();

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
	// SpawnLocation.Z = SVLocation.Z + 100.0f;

	float Prob = FMath::RandRange(0.0f, 1.0f);
	float Sum{};

	for (FSpawnEntry SpawnEntry : SpawnEntries)
	{
		Sum += SpawnEntry.Prob;

		if (Sum >= Prob)
		{
			FName MonsterID = SpawnEntry.ID;

			// Get Monster Data
			FMonsterInfo MonsterData{};
			GameInstance->GetMonsterData(MonsterID, MonsterData);

			// Spawn AI
			MonsterSpawnSystem->MonsterSpawn(SpawnLocation, MonsterID);

			if (IsChase)
			{
				// Chase Player
				MonsterCallingSystem->CallMonsters(SVLocation, SpawnMaxRadius, Character, ECallType::Attack);
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

	UE_LOG(LogGameFlow, Warning, TEXT("Start Phase"));
	UE_LOG(LogGameFlow, Warning, TEXT("Phase Count : %d"), Size);

	SpawnPhaseMonsters();
}

void ASpawnVolume::SpawnPhaseMonsters()
{
	int32 Size = PhaseData.PhaseEntries.Num();

	UE_LOG(LogGameFlow, Warning, TEXT("Spawn Phase Monster"));

	TArray<FPhaseEntry> PhaseEntries = PhaseData.PhaseEntries;
	TArray<FSpawnEntry> SpawnEntries = PhaseEntries[PhaseIndex].SpawnEntries;
	int32 SpawnCount = PhaseEntries[PhaseIndex].SpawnCount;

	UE_LOG(LogGameFlow, Warning, TEXT("Phase Monster Count : %d"), SpawnCount);

	for (int i = 0; i < SpawnCount; i++)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Spawn Phase Random Monster"));
		SpawnRandomMonster(SpawnEntries, SpawnMinRadius, SpawnMaxRadius, true);
	}

	if (PhaseIndex == Size - 1)
	{
		UE_LOG(LogGameFlow, Warning, TEXT("End Phase"));
		PhaseIndex = 0;

		return;
	}

	float Duration = PhaseData.PhaseEntries[PhaseIndex].Duration;

	GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &ASpawnVolume::StartPhase, Duration, false);

	PhaseIndex += 1;
}

void ASpawnVolume::ActivateContainers()
{
	if (!GetGameInstance())
	{
		return;
	}

	if (UContainerManager* ContainerManager = GetGameInstance()->GetSubsystem<UContainerManager>())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Spawn Volume : Activate Containers"));

		ContainerManager->ActivateContainers(this);
	}
}

FName ASpawnVolume::GetRegionID() const
{
	return RegionID;
}

void ASpawnVolume::CleanSetting()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PhaseTimer);
	}
}
