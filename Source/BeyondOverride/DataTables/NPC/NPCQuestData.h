// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NPCQuestData.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName NPCID;
};
