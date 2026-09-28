// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameFlow/NPC/ActorComponent/NPCActorComponent.h"

#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UInteractionComponent : public UNPCActorComponent
{
	GENERATED_BODY()

  public:
	UInteractionComponent();

  protected:
	virtual void BeginPlay() override;

  public:
	void InteractNPC();
};
