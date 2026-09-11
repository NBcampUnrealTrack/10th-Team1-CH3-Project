// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "ContainerManager.generated.h"

/**
 *
 */
UCLASS()
class BEYONDOVERRIDE_API UContainerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	void Initialize();
	void ActivateContainer();

  public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mananer")
	float ActivateProbability;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mananer")
	TMap<FName, bool> LimitedItems;

  private:
	TArray<TObjectPtr<AActor>> Containers;  // AContainer·Î º¯°æ
};
