// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../BOEnums.h"
#include "UObject/NoExportTypes.h"

#include "BaseFarmingState.generated.h"

class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API UBaseFarmingState : public UObject
{
	GENERATED_BODY()

  public:
	virtual void Initialize(UFarmingStateMachine* InStateMachine);
	virtual void Enter();
	virtual void Exit();
	virtual void ChangeState(EFarmingState FarmingState) const;

	// EFarmingState GetState() const;

  protected:
	TObjectPtr<UFarmingStateMachine> StateMachine;
};
