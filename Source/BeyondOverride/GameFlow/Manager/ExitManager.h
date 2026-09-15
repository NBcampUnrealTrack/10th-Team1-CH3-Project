// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "ExitManager.generated.h"

class AExitActor;
class AExitControllerActor;

UCLASS()
class BEYONDOVERRIDE_API UExitManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  public:
	void InitSetting();
	void SpawnCharacter();
	void ActivateExit();
	AExitActor* SelectRandomExit();

	UFUNCTION(BlueprintCallable, Category = "Exit")
	void HandleExtract(AExitActor* ExitPoint, AActor* Interactor);

  private:
	float ExitActivateProb;
	TArray<TObjectPtr<AExitActor>> Exits;
	TArray<TObjectPtr<AExitControllerActor>> ExitControllers;
};
