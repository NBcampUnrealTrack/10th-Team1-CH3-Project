// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumFarmingState.h"

#include "UObject/NoExportTypes.h"

#include "BaseFarmingState.generated.h"

class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API UBaseFarmingState : public UObject
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UFarmingStateMachine* InStateMachine);
	virtual void EnterState();
	virtual void ExitState();
	virtual void ChangeState(EFarmingState FarmingState) const;

	EFarmingState GetState() const;

  private:
	TObjectPtr<UFarmingStateMachine> StateMachine;
};
