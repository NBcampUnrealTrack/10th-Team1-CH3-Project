// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/NPCEnums.h"

#include "NPCDialogueData.generated.h"

USTRUCT(BlueprintType)
struct FTopicData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Topic = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NPCID = "None";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETalkSituation TalkSituation = ETalkSituation::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETalkType TalkType = ETalkType::Once;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextDialogueID = "None";
};

USTRUCT(BlueprintType)
struct FDialogueInteractionData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NPCID = "None";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueSituation DialogueSituation = EDialogueSituation::Idle;
};

USTRUCT(BlueprintType)
struct FTalkOption
{
	GENERATED_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText OptionText = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextDialogueID = "None";
};

USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

  public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text = FText::FromString(TEXT("None"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTalkOption> Options;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextDialogueID = "None";
};
