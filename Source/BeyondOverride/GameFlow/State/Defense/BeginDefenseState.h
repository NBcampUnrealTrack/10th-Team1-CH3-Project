// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/Defense/BaseDefenseState.h"

#include "BeginDefenseState.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBeginDefenseState : public UBaseDefenseState
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UBaseStateMachine* InStateMachine) override;
	virtual void Enter() override;

  private:
	void SpawnSupplies();

  private:
	int32 SpawnCount;

	TArray<FName> Supplies;
	TArray<FVector> SupplySpawnLocations;
};
