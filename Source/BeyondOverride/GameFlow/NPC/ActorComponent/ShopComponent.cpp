// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFlow/NPC/ActorComponent/ShopComponent.h"

UShopComponent::UShopComponent()
{
	Option = ENPCInteractionOption::Shop;
}

void UShopComponent::BeginPlay()
{
	Super::BeginPlay();
}
