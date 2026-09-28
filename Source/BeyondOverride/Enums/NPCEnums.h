// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NPCEnums.generated.h"

UENUM(BlueprintType)
enum class ENPCInteractionOption : uint8
{
	None UMETA(DisplayName = "???"),
	Dialogue UMETA(DisplayName = "대화"),
	Shop UMETA(DisplayName = "상점"),
	Quest UMETA(DisplayName = "의뢰"),
	Heal UMETA(DisplayName = "회복"),
	Exit UMETA(DisplayName = "(돌아가기)"),
};

UENUM(BlueprintType)
enum class EDialogueSituation : uint8
{
	None,
	Idle,
	Talk,
	Quest
};

UENUM(BlueprintType)
enum class ETalkType : uint8
{
	Once,
	Repeatable,
};

UENUM(BlueprintType)
enum class ETalkSituation : uint8
{
	None,
	Greeting,
	Idle
};
