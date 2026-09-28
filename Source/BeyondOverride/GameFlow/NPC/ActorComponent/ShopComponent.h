// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameFlow/NPC/ActorComponent/NPCActorComponent.h"

#include "ShopComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BEYONDOVERRIDE_API UShopComponent : public UNPCActorComponent
{
	GENERATED_BODY()

  public:
	UShopComponent();

  protected:
	virtual void BeginPlay() override;

  private:
	// FNPCShopData NPCShopData;
};
