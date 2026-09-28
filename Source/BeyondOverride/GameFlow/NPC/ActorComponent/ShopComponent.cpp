// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/ShopComponent.h"

#include "GameFlow/Manager/NPCManager.h"
#include "GameFlow/NPC/NPCBase.h"

UShopComponent::UShopComponent()
{
	// Option = ENPCInteractionOption::Shop;
}

void UShopComponent::BeginPlay()
{
	/*Super::BeginPlay();

	if (!NPC || !GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}

	UNPCManager* NPCManager = GetWorld()->GetGameInstance()->GetSubsystem<UNPCManager>();
	if (!NPCManager)
	{
		return;
	}

	FName NPCID = NPC->GetNPCID();
	NPCManager->GetNPCShopData(NPCID, NPCShopData);*/
}
