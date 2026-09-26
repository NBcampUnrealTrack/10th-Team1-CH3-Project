// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "UObject/NoExportTypes.h"

#include "BaseState.generated.h"

class UBaseStateMachine;

UCLASS()
class BEYONDOVERRIDE_API UBaseState : public UObject
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UBaseStateMachine* InStateMachine);
	virtual void InitSetting();
	virtual void Enter();
	virtual void Exit();
	virtual void ChangeState(EStageState StageState) const;
};
