// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Defense/BeginDefenseState.h"

#include "DefenseStateMachine.h"

#include "Algo/RandomShuffle.h"
#include "DataAssets/BODataAsset.h"
#include "Factory/ItemFactory.h"
#include "GameFlow/BOGameInstance.h"
#include "Logging/BOLog.h"

void UBeginDefenseState::Initialize(UBaseStateMachine* InStateMachine)
{
	Super::Initialize(InStateMachine);

	if (!GetWorld() || !DefenseStateMachine)
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

	DataAsset->GetSupplies(Supplies);

	DefenseStateMachine->GetSupplySpawnLocations(SupplySpawnLocations);

	SpawnCount = FMath::Min(DataAsset->GetSupplySpawnCount(), SupplySpawnLocations.Num());
}

void UBeginDefenseState::Enter()
{
	UE_LOG(LogGameFlow, Warning, TEXT("Defense Begin"));
	Super::Enter();

	SpawnSupplies();

	if (DefenseStateMachine)
	{
		DefenseStateMachine->ChangeState(EStageState::Progress);
	}
}

void UBeginDefenseState::SpawnSupplies()
{
	Algo::RandomShuffle(Supplies);

	FItemFactory ItemFactory{};

	for (int32 i = 0; i < SpawnCount; i++)
	{
		FName ItemID = Supplies[i];
		FVector Location = SupplySpawnLocations[i];

		ItemFactory.SpawnItemPickup(GetWorld(), ItemID, 1, Location);
	}
}
