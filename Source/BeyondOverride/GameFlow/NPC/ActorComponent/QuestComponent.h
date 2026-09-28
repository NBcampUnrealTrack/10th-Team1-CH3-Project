// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameFlow/NPC/ActorComponent/NPCActorComponent.h"

#include "QuestComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UQuestComponent : public UNPCActorComponent
{
	GENERATED_BODY()

  public:
	UQuestComponent();

  protected:
	virtual void BeginPlay() override;
};
