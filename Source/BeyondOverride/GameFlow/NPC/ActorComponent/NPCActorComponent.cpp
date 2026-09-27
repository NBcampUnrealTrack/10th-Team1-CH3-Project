// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/NPCActorComponent.h"

#include "GameFlow/NPC/NPCBase.h"

UNPCActorComponent::UNPCActorComponent()
{
}

void UNPCActorComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSetting();
}

void UNPCActorComponent::InitSetting()
{
	if (AActor* Actor = GetOwner())
	{
		NPC = Cast<ANPCBase>(Actor);

		if (NPC && Option != ENPCInteractionOption::None)
		{
			NPC->AddInteractionOption(Option);
		}
	}
}
