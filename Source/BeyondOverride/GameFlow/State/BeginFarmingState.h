// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/BaseFarmingState.h"

#include "BeginFarmingState.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UBeginFarmingState : public UBaseFarmingState
{
	GENERATED_BODY()

  public:
	virtual void EnterState() override;
	virtual void ExitState() override;

  private:
	void SpawnCharacter();
	void SpawnMonster();
};
