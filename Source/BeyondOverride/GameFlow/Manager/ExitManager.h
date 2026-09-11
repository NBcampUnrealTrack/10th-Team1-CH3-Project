// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "ExitManager.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UExitManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	void Initialize();
	void ActivateExit();
	AActor* SelectRandomExit();
};
