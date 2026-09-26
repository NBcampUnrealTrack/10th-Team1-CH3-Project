// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFlow/State/Defense/BaseDefenseState.h"

#include "EndDefenseState.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UEndDefenseState : public UBaseDefenseState
{
	GENERATED_BODY()

  public:
	virtual void Enter() override;
};
