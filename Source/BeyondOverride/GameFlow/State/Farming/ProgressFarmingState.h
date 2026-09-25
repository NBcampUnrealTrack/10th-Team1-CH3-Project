// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/Farming/BaseFarmingState.h"

#include "ProgressFarmingState.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UProgressFarmingState : public UBaseFarmingState
{
	GENERATED_BODY()

  public:
	virtual void Enter() override;

  private:
	void SpawnCharacter();
	void TeleportCharacter();
	void ActivateExits();
	void SetStartTime();
};
