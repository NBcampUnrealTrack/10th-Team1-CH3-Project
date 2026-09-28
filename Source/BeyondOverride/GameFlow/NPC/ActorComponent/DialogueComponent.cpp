// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/DialogueComponent.h"

UDialogueComponent::UDialogueComponent()
{
	Option = ENPCInteractionOption::Dialogue;
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}
