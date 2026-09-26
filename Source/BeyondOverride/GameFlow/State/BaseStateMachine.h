// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/BOEnums.h"
#include "UObject/NoExportTypes.h"

#include "BaseStateMachine.generated.h"

class ABOGameMode;
class UBaseState;

UCLASS()
class BEYONDOVERRIDE_API UBaseStateMachine : public UObject
{
	GENERATED_BODY()

  public:
	UBaseStateMachine();

	virtual void Initialize(ABOGameMode* InGameMode);
	virtual void ChangeState(EStageState StageState);
	virtual void SetStageResult(EStageResult Result);

	virtual EStageResult GetStageResult() const;

  protected:
	virtual void CreateState(EStageState State);

  protected:
	TObjectPtr<ABOGameMode> GameMode;
	EStageState CurrentStageState;
	EStageResult StageResult;
};
