// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/QuestComponent.h"

UQuestComponent::UQuestComponent()
{
	Option = ENPCInteractionOption::Quest;
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();
}
