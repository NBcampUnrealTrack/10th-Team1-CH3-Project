// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumFarmingState.h"

#include "UObject/NoExportTypes.h"

#include "FarmingStateMachine.generated.h"

class ABOGameMode;
class UBaseFarmingState;

UCLASS()
class BEYONDOVERRIDE_API UFarmingStateMachine : public UObject
{
	GENERATED_BODY()

  public:
	void Initialize(ABOGameMode* InGameMode);
	void ChangeState(EFarmingState FarmingState);

  private:
	void CreateState(EFarmingState FarmingState);

  public:
	TObjectPtr<ABOGameMode> GameMode;
	TObjectPtr<UBaseFarmingState> CurrentState;
	EFarmingState CurrentFarmingState;
};
