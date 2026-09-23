// 26/09/19 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLSoundEvent.h"

// Add include
#include "DataTables/Interface/SoundPlayData.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Sound/SoundBase.h"

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
	if (!Player)
	{
		return;
	}

	UAISense_Hearing::ReportNoiseEvent(World,
									   Location,
									   1.0f,
									   Player,
									   Radius,
									   FName("PlayerOwnerSound!"));
}

void UBFLSoundEvent::SoundPlay(FVector Location, FName SoundTarget, float Radius, UObject* WorldContextObject)
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

	UDataTable* SoundDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_SoundPlay.DT_SoundPlay"));
	if (!SoundDataTable)
	{
		return;
	}

	FSoundPlayData* SoundData = SoundDataTable->FindRow<FSoundPlayData>(SoundTarget, TEXT("SoundPlay"));
	if (!SoundData)
	{
		return;
	}

	NoisePlay(Location, Radius, WorldContextObject);

	USoundBase* TargetSound = SoundData->SoundTarget;

	UGameplayStatics::PlaySoundAtLocation(World,
										  TargetSound,
										  Location,
										  1.0f,
										  1.0f);
}

void UBFLSoundEvent::SoundPlay(FVector Location, FName SoundTarget, float Radius, float Pitch, float Volume, UObject* WorldContextObject)
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

	UDataTable* SoundDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_SoundPlay.DT_SoundPlay"));
	if (!SoundDataTable)
	{
		return;
	}

	FSoundPlayData* SoundData = SoundDataTable->FindRow<FSoundPlayData>(SoundTarget, TEXT("SoundPlay"));
	if (!SoundData)
	{
		return;
	}

	NoisePlay(Location, Radius, WorldContextObject);

	USoundBase* TargetSound = SoundData->SoundTarget;

	UGameplayStatics::PlaySoundAtLocation(World,
										  TargetSound,
										  Location,
										  Volume,
										  Pitch);
}

void UBFLSoundEvent::SoundPlay(FVector Location, FName SoundTarget, float Radius, bool IsPlayer, UObject* WorldContextObject)
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

	UDataTable* SoundDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_SoundPlay.DT_SoundPlay"));
	if (!SoundDataTable)
	{
		return;
	}

	FSoundPlayData* SoundData = SoundDataTable->FindRow<FSoundPlayData>(SoundTarget, TEXT("SoundPlay"));
	if (!SoundData)
	{
		return;
	}

	if (IsPlayer)
	{
		NoisePlay(Location, Radius, WorldContextObject);
	}

	USoundBase* TargetSound = SoundData->SoundTarget;

	UGameplayStatics::PlaySoundAtLocation(World,
										  TargetSound,
										  Location,
										  1.0f, // VolumeMultiplier
										  1.0f  // PitchMultiplier
	);
}

void UBFLSoundEvent::SoundPlay(FVector Location, FName SoundTarget, float Radius, float Pitch, float Volume, bool IsPlayer, UObject* WorldContextObject)
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

	UDataTable* SoundDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_SoundPlay.DT_SoundPlay"));
	if (!SoundDataTable)
	{
		return;
	}

	FSoundPlayData* SoundData = SoundDataTable->FindRow<FSoundPlayData>(SoundTarget, TEXT("SoundPlay"));
	if (!SoundData)
	{
		return;
	}

	if (IsPlayer)
	{
		NoisePlay(Location, Radius, WorldContextObject);
	}

	USoundBase* TargetSound = SoundData->SoundTarget;

	UGameplayStatics::PlaySoundAtLocation(World,
										  TargetSound,
										  Location,
										  Volume, // VolumeMultiplier
										  Pitch   // PitchMultiplier
	);
}
