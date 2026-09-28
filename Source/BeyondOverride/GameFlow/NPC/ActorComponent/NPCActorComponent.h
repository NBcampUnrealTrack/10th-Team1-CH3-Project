// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Enums/NPCEnums.h"

#include "NPCActorComponent.generated.h"

class ANPCBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UNPCActorComponent : public UActorComponent
{
	GENERATED_BODY()

  public:
	UNPCActorComponent();

  protected:
	virtual void BeginPlay() override;

  public:
	void InitSetting();

  protected:
	UPROPERTY()
	ENPCInteractionOption Option;

	UPROPERTY()
	TObjectPtr<ANPCBase> NPC;
};
