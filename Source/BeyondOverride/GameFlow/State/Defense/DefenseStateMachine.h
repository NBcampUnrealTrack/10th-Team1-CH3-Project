// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Defense/DefenseData.h"
#include "GameFlow/State/BaseStateMachine.h"

#include "DefenseStateMachine.generated.h"

class UBaseDefenseState;
class UBeginDefenseState;
class UProgressDefenseState;
class UEndDefenseState;

UCLASS()
class BEYONDOVERRIDE_API UDefenseStateMachine : public UBaseStateMachine
{
	GENERATED_BODY()

  public:
	UDefenseStateMachine();

  private:
	void LoadDefenseData();
	void SetSpawnLocations();

  public:
	virtual void ChangeState(EStageState StageState) override;
	void OnPhaseEnded();

	void GetDefenseData(FDefenseData& Data) const;
	void GetSupplySpawnLocations(TArray<FVector>& Locations) const;
	void GetMonsterSpawnLocations(TArray<FVector>& Locations) const;

  private:
	// virtual void CreateState(EStageState StageState) override;

  private:
	int32 PhaseIndex;

	UPROPERTY()
	TObjectPtr<UBaseDefenseState> CurrentDefenseState;
	UPROPERTY()
	TObjectPtr<UBeginDefenseState> BeginDefenseState;
	UPROPERTY()
	TObjectPtr<UProgressDefenseState> ProgressDefenseState;
	UPROPERTY()
	TObjectPtr<UEndDefenseState> EndDefenseState;

	TMap<int32, FDefenseData> DefenseDatas;
	TArray<FVector> SupplySpawnLocations;
	TArray<FVector> MonsterSpawnLocations;
};
