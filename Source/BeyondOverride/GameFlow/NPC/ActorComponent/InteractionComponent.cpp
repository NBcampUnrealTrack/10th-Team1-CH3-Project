// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/InteractionComponent.h"

#include "UI/Manager/UIManager.h"

UInteractionComponent::UInteractionComponent()
{
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::InteractNPC()
{
	//// show interaction widget
	// if (!GetWorld() || !GetWorld()->GetGameInstance())
	//{
	//	return;
	// }

	// UUIManager* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();
	// if (!UIManager)
	//{
	//	return;
	// }

	// UIManager->PushScreen(EUIScreen::NPCInteraction, EUIInputMode::UIOnly);
}
