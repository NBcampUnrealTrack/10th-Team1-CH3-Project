// Fill out your copyright notice in the Description page of Project Settings.

#include "BOWorldSubsystem.h"

void UBOWorldSubsystem::SetStartTime()
{
	if (GetWorld())
	{
		StartTime = GetWorld()->GetTimeSeconds();
	}
}

void UBOWorldSubsystem::SetEndTime()
{
	if (GetWorld())
	{
		EndTime = GetWorld()->GetTimeSeconds();
	}
}

float UBOWorldSubsystem::GetSurvivalTime() const
{
	return EndTime - StartTime;
}
