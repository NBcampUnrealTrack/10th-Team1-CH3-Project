// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/BaseState.h"

#include "BaseDefenseState.generated.h"

class UDefenseStateMachine;

UCLASS()
class BEYONDOVERRIDE_API UBaseDefenseState : public UBaseState
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UBaseStateMachine* InStateMachine) override;
	virtual void InitSetting() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ChangeState(EStageState StageState) const override;

  protected:
	TObjectPtr<UDefenseStateMachine> DefenseStateMachine;
};
