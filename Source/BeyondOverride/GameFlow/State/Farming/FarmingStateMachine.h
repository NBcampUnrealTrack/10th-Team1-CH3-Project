// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "GameFlow/State/BaseStateMachine.h"
#include "UObject/NoExportTypes.h"

#include "FarmingStateMachine.generated.h"

class UBaseFarmingState;

UCLASS()
class BEYONDOVERRIDE_API UFarmingStateMachine : public UBaseStateMachine
{
	GENERATED_BODY()

  public:
	UFarmingStateMachine();

	virtual void ChangeState(EStageState StageState) override;

  private:
	virtual void CreateState(EStageState StageState) override;

  private:
	TObjectPtr<UBaseFarmingState> CurrentFarmingState;
};
