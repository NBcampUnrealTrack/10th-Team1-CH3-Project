// Fill out your copyright notice in the Description page of Project Settings.

#include "BOWorldSubsystem.h"

#include "Logging/BOLog.h"

void UBOWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogGameFlow, Warning, TEXT("World Subsystem Initialized"));
}

void UBOWorldSubsystem::SetStartTime()
{
	if (GetWorld())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("Start Set Time : %f"), GetWorld()->GetTimeSeconds());
		StartTime = GetWorld()->GetTimeSeconds();
	}
}

void UBOWorldSubsystem::SetEndTime()
{
	if (GetWorld())
	{
		UE_LOG(LogGameFlow, Warning, TEXT("End Set Time : %f"), GetWorld()->GetTimeSeconds());
		TotalTime = GetWorld()->TimeSince(StartTime);
	}
}

float UBOWorldSubsystem::GetSurvivalTime() const
{
	if (GetWorld())
	{
		return GetWorld()->TimeSeconds;
	}

	return 0.0f;
}
