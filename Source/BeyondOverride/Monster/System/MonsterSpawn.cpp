
// 26/09/15 Copyright Jinho Song

// Base include
#include "Monster/System/MonsterSpawn.h"

// Add include
#include "MonsterSpawn.h"
#include "NavigationSystem.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

void UMonsterSpawn::MonsterSpawn(TSubclassOf<AMonsterCharacter> MonsterClass, FVector Location, FName ID)
{

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	FVector MoveLocation;

	if (NavSystem && !NavSystem->ProjectPointToNavigation(Location, NavLocation))
	{
		float Radius = 200;

		for (int i = 0; i < 36; ++i)
		{
			float Angle = FMath::DegreesToRadians(i * 10.0f);

			FVector Point = Location;

			Point.X += FMath::Cos(Angle) * Radius;
			Point.Y += FMath::Sin(Angle) * Radius;

			// Point = Center에서 정확히 Radius만큼 떨어진 위치
			if (NavSystem->ProjectPointToNavigation(Point, NavLocation, FVector(200, 200, 2000.0f)))
			{

				MoveLocation = NavLocation.Location;
				break;
			}
		}
	}
	else
	{
		MoveLocation = Location;
	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.CustomPreSpawnInitalization = [ID](AActor* SpawnedActor)
	{
		AMonsterCharacter* Monster = Cast<AMonsterCharacter>(SpawnedActor);

		if (Monster)
		{
			Monster->SetMonsterID(ID);
		}
	};

	AMonsterCharacter* SpawnedActor = GetWorld()->SpawnActor<AMonsterCharacter>(MonsterClass,
																				MoveLocation,
																				FRotator::ZeroRotator,
																				SpawnParams);
}
