// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/NPCEnums.h"

#include "NPCData.generated.h"

USTRUCT(BlueprintType)
struct FNPCData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName NPCName = FName(TEXT("Default"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TArray<ENPCInteractionOption> InteractionOptions;
};

USTRUCT(BlueprintType)
struct FNPCShopData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<FName> ShopItems;
};
