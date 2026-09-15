// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../Enums/BOEnums.h"
#include "UObject/NoExportTypes.h"

#include "FarmingStateMachine.generated.h"

class ABOGameMode;
class UBaseFarmingState;

UCLASS()
class BEYONDOVERRIDE_API UFarmingStateMachine : public UObject
{
	GENERATED_BODY()

  public:
	UFarmingStateMachine();

	void Initialize(ABOGameMode* InGameMode);
	void ChangeState(EFarmingState FarmingState);
	void SetFarmingResult(EFarmingResult Result);

	EFarmingResult GetFarmingResult() const;

  private:
	void CreateState(EFarmingState FarmingState);

  public:
	TObjectPtr<ABOGameMode> GameMode;
	TObjectPtr<UBaseFarmingState> CurrentState;
	EFarmingState CurrentFarmingState;
	EFarmingResult FarmingResult;
};
