// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataTables/Defense/DefenseData.h"
#include "GameFlow/State/Defense/BaseDefenseState.h"

#include "ProgressDefenseState.generated.h"

class ABOCharacter;
class UMonsterSpawn;
class UMonsterCalling;

UCLASS()
class BEYONDOVERRIDE_API UProgressDefenseState : public UBaseDefenseState
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UBaseStateMachine* StateMachine) override;
	virtual void Enter() override;

  private:
	void SetDefenseData();
	void PrepareMonsterSpawn();

	UFUNCTION(BlueprintCallable, Category = "Progress")
	void SpawnMonster();

	UFUNCTION(BlueprintCallable, Category = "Progress")
	void OnPhaseEnded();

  private:
	FDefenseData DefenseData;

	float TotalDefenseTime;
	int32 SpawnInterval;

	UPROPERTY()
	TObjectPtr<ABOCharacter> Character;
	UPROPERTY()
	TObjectPtr<UMonsterSpawn> MonsterSpawn;
	UPROPERTY()
	TObjectPtr<UMonsterCalling> MonsterCalling;

	TArray<FVector> MonsterSpawnLocations;

	FTimerHandle PhaseTimer;
	FTimerHandle SpawnTimer;
};
