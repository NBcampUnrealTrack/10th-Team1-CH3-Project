// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enums/NPCEnums.h"

DECLARE_DELEGATE_OneParam(FOnInteractionButtonClicked, ENPCInteractionOption);
DECLARE_DELEGATE_OneParam(FOnTalkButtonClicked, FName);
