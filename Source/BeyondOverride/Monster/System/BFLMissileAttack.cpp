// 26/09/23 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLMissileAttack.h"

// Add include
#include "Monster/Actor/AttackMissileActor.h"

void UBFLMissileAttack::MissileAttack(FVector SpawnLocation,
									  FVector AttackPoint,
									  float FireAngle,
									  float FlightTime,
									  ACharacter* ThisOwner,
									  UObject* WorldContextObject)
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

	UBlueprint* MissileBlueprint = LoadObject<UBlueprint>(
		nullptr,
		TEXT("/Game/Blueprints/Monster/AttackMissile/BP_AttackMissileActor.BP_AttackMissileActor"));

	if (!MissileBlueprint)
	{
		return;
	}

	UClass* MissileClass = MissileBlueprint->GeneratedClass;

	if (!MissileClass || !MissileClass->IsChildOf(AAttackMissileActor::StaticClass()))
	{
		return;
	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.CustomPreSpawnInitalization = [AttackPoint, FireAngle, FlightTime, ThisOwner](AActor* SpawnedActor)
	{
		AAttackMissileActor* AttackMissile = Cast<AAttackMissileActor>(SpawnedActor);

		if (AttackMissile)
		{
			AttackMissile->MissileSetUp(AttackPoint, FireAngle, FlightTime, ThisOwner);
		}
	};

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAttackMissileActor* SpawnedActor = World->SpawnActor<AAttackMissileActor>(MissileClass,
																			   SpawnLocation,
																			   FRotator::ZeroRotator,
																			   SpawnParams);
}
