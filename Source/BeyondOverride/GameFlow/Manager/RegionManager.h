// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "RegionManager.generated.h"

class UBODataAsset;

UCLASS()
class BEYONDOVERRIDE_API URegionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	void InitSetting();
	void CleanSetting();
};
