// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/State/Defense/DefenseStateMachine.h"

#include "BaseDefenseState.h"
#include "BeginDefenseState.h"
#include "EndDefenseState.h"
#include "ProgressDefenseState.h"

#include "DataAssets/BODataAsset.h"
#include "Engine/TargetPoint.h"
#include "GameFlow/BOGameInstance.h"
#include "GameFlow/BOGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/BOLog.h"

UDefenseStateMachine::UDefenseStateMachine()
	: PhaseIndex(0),
	  CurrentDefenseState(nullptr),
	  BeginDefenseState(nullptr),
	  ProgressDefenseState(nullptr),
	  EndDefenseState(nullptr)
{
	DefenseDatas.Empty();
	SupplySpawnLocations.Empty();
	MonsterSpawnLocations.Empty();

	LoadDefenseData();
	SetSpawnLocations();
}

void UDefenseStateMachine::LoadDefenseData()
{
	if (!GetWorld())
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

	UDataTable* DefenseDataTable = DataAsset->GetDefenseDataTable();
	if (!DefenseDataTable)
	{
		return;
	}

	TMap<FName, uint8*> AllRows = DefenseDataTable->GetRowMap();

	for (const TPair<FName, uint8*>& Pair : AllRows)
	{
		int32 Index = FCString::Atoi(*Pair.Key.ToString());
		FDefenseData* DefenseData = reinterpret_cast<FDefenseData*>(Pair.Value);

		DefenseDatas.Add(Index, *DefenseData);
	}
}

void UDefenseStateMachine::SetSpawnLocations()
{
	if (!GetWorld())
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

		if (TargetPoint->ActorHasTag(FName(TEXT("Supply"))))
		{
			FVector Location = TargetPoint->GetActorLocation();

			SupplySpawnLocations.Add(Location);
		}
		else if (TargetPoint->ActorHasTag(FName(TEXT("Monster"))))
		{
			FVector Location = TargetPoint->GetActorLocation();

			MonsterSpawnLocations.Add(Location);
		}
	}
}

void UDefenseStateMachine::OnPhaseEnded()
{
	PhaseIndex += 1;

	if (PhaseIndex == DefenseDatas.Num())
	{
		if (GameMode)
		{
			GameMode->EndDefense();
		}
	}
	else
	{
		ChangeState(EStageState::Begin);
	}
}

void UDefenseStateMachine::GetDefenseData(FDefenseData& Data) const
{
	if (DefenseDatas.Contains(PhaseIndex))
	{
		Data = DefenseDatas[PhaseIndex];
	}
}

void UDefenseStateMachine::GetSupplySpawnLocations(TArray<FVector>& Locations) const
{
	Locations = SupplySpawnLocations;
}

void UDefenseStateMachine::GetMonsterSpawnLocations(TArray<FVector>& Locations) const
{
	Locations = MonsterSpawnLocations;
}

void UDefenseStateMachine::ChangeState(EStageState StageState)
{
	Super::ChangeState(StageState);

	CurrentStageState = StageState;
	/*CreateState(CurrentStageState);

	if (CurrentDefenseState)
	{
		CurrentDefenseState->Initialize(this);
		CurrentDefenseState->Enter();
	}*/

	switch (CurrentStageState)
	{
	case EStageState::Begin:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Begin"));
		if (!IsValid(BeginDefenseState))
		{
			BeginDefenseState = NewObject<UBeginDefenseState>(this, UBeginDefenseState::StaticClass());
			BeginDefenseState->Initialize(this);
		}
		CurrentDefenseState = BeginDefenseState;
		break;
	}
	case EStageState::Progress:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Progress"));
		if (!IsValid(ProgressDefenseState))
		{
			ProgressDefenseState = NewObject<UProgressDefenseState>(this, UProgressDefenseState::StaticClass());
			ProgressDefenseState->Initialize(this);
		}
		CurrentDefenseState = ProgressDefenseState;
		break;
	}
	case EStageState::End:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State End"));
		if (!IsValid(EndDefenseState))
		{
			EndDefenseState = NewObject<UEndDefenseState>(this, UEndDefenseState::StaticClass());
			EndDefenseState->Initialize(this);
		}
		CurrentDefenseState = EndDefenseState;
		break;
	}
	default:
		break;
	}

	if (CurrentDefenseState)
	{
		CurrentDefenseState->Enter();
	}
}

/*
void UDefenseStateMachine::CreateState(EStageState StageState)
{
	Super::CreateState(StageState);

	switch (StageState)
	{
	case EStageState::Begin:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Begin"));
		CurrentDefenseState = NewObject<UBeginDefenseState>(this, UBeginDefenseState::StaticClass());
		break;
	}
	case EStageState::Progress:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State Progress"));
		CurrentDefenseState = NewObject<UProgressDefenseState>(this, UProgressDefenseState::StaticClass());
		break;
	}
	case EStageState::End:
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Farming State End"));
		CurrentDefenseState = NewObject<UEndDefenseState>(this, UEndDefenseState::StaticClass());
		break;
	}
	default:
		break;
	}
}
*/
