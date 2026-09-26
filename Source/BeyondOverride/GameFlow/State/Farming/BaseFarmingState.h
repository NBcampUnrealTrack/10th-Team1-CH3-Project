// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "GameFlow/State/BaseState.h"
#include "UObject/NoExportTypes.h"

#include "BaseFarmingState.generated.h"

class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API UBaseFarmingState : public UBaseState
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UBaseStateMachine* InStateMachine) override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ChangeState(EStageState StageState) const override;

  protected:
	TObjectPtr<UFarmingStateMachine> FarmingStateMachine;
};
