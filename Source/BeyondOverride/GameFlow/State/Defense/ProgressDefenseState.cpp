// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Defense/ProgressDefenseState.h"

#include "DefenseStateMachine.h"

#include "DataAssets/BODataAsset.h"
#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"
#include "Monster/System/MonsterCalling.h"
#include "Monster/System/MonsterSpawn.h"
#include "Player/Character/BOCharacter.h"

void UProgressDefenseState::Initialize(UBaseStateMachine* StateMachine)
{
	Super::Initialize(StateMachine);

	if (!GetWorld() || !GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	UBOGameInstance* GameInstance = GetWorld()->GetGameInstance<UBOGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	UBODataAsset* DataAsset = GameInstance->GetBODataAsset();
	if (!DataAsset)
	{
		return;
	}

	Character = GetWorld()->GetFirstPlayerController()->GetPawn<ABOCharacter>();
	MonsterSpawn = NewObject<UMonsterSpawn>(this, UMonsterSpawn::StaticClass());
	MonsterCalling = NewObject<UMonsterCalling>(this, UMonsterCalling::StaticClass());

	TotalDefenseTime = DataAsset->GetTotalDefenseTime();
	SpawnInterval = DataAsset->GetMonsterSpawnInterval();

	if (DefenseStateMachine)
	{
		DefenseStateMachine->GetMonsterSpawnLocations(MonsterSpawnLocations);
	}
}

void UProgressDefenseState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Defense Progress Enter"));
	Super::Enter();

	SetDefenseData();
	PrepareMonsterSpawn();
}

void UProgressDefenseState::SetDefenseData()
{
	if (DefenseStateMachine)
	{
		DefenseStateMachine->GetDefenseData(DefenseData);
	}
}

void UProgressDefenseState::PrepareMonsterSpawn()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(PhaseTimer);
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	float DurationRatio = DefenseData.DurationRatio;
	float Duration = TotalDefenseTime * DurationRatio;
	GetWorld()->GetTimerManager().SetTimer(PhaseTimer, this, &UProgressDefenseState::OnPhaseEnded, Duration, false);

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &UProgressDefenseState::SpawnMonster, SpawnInterval, true);
}

void UProgressDefenseState::SpawnMonster()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Defense SpawnMonster Called"));
	TArray<FSpawnEntry> SpawnEntries = DefenseData.SpawnEntries;
	int32 Size = SpawnEntries.Num();
	float Prob{};
	float Sum{};

	for (FVector Location : MonsterSpawnLocations)
	{
		Prob = FMath::RandRange(0.0f, 1.0f);
		Sum = 0.0f;

		for (int32 i = 0; i < Size; i++)
		{
			Sum += SpawnEntries[i].Prob;

			if (Sum >= Prob)
			{
				FName MonsterID = SpawnEntries[i].ID;

				UE_LOG(LogGameFlow, Warning, TEXT("Defense Spawn Monster : %s"), *MonsterID.ToString());
				MonsterSpawn->MonsterSpawn(Location, MonsterID);
			}
		}
	}

	ABOCharacter* Target = Character.Get();
	FVector PlayerLocation = Character->GetActorLocation();
	MonsterCalling->CallMonsters(PlayerLocation, 10000.0f, Target, ECallType::Attack);
}

void UProgressDefenseState::OnPhaseEnded()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	if (DefenseStateMachine)
	{
		DefenseStateMachine->ChangeState(EStageState::End);
	}
}
