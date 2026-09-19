// 26/09/19 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLSoundEvent.h"

// Add include
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Hearing.h"

void UBFLSoundEvent::NoisePlay(FVector Location, float Radius, UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!World)
	{
		return;
	}

	APawn* Player = UGameplayStatics::GetPlayerPawn(World, 0);

	UAISense_Hearing::ReportNoiseEvent(World,
									   Location,
									   1.0f,
									   Player,
									   Radius,
									   FName("PlayerOwnerSound!"));
}

void SoundPlay(FVector Location, FName SoundTarget, float Radius, UObject* WorldContextObject)
{
}
