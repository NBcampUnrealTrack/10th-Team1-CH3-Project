// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/Farming/BaseFarmingState.h"

#include "EndFarmingState.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEndFarmingState : public UBaseFarmingState
{
	GENERATED_BODY()

  public:
	virtual void Enter() override;

  private:
	void SetEndTime();
	void CleanRegions();
	void SetFarmingResult();
};
