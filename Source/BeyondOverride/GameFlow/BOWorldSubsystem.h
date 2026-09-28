// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "BOWorldSubsystem.generated.h"

UCLASS()
class BEYONDOVERRIDE_API UBOWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

  public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetStartTime();
	void SetEndTime();

	float GetSurvivalTime() const;

  private:
	float StartTime;
	float TotalTime;
};
