// Fill out your copyright notice in the Description page of Project Settings.

#include "BOWorldSubsystem.h"

float UBOWorldSubsystem::GetSurvivalTime() const
{
	return EndTime - StartTime;
}
