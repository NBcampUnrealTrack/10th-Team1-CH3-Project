// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BOEnums.h"
#include "CoreMinimal.h"

#include "GameFramework/GameMode.h"

#include "BOGameMode.generated.h"

class UFarmingStateMachine;

UCLASS()
class BEYONDOVERRIDE_API ABOGameMode : public AGameMode
{
	GENERATED_BODY()

  public:
	ABOGameMode();

	virtual void BeginPlay() override;

	void StartFarming();
	void EndFarming(EFarmingResult Result);

  private:
	TObjectPtr<UFarmingStateMachine> StateMachine;
};
