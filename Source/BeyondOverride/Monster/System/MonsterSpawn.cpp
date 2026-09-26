
// 26/09/15 Copyright Jinho Song

// Base include
#include "Monster/System/MonsterSpawn.h"

// Add include
#include "MonsterSpawn.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BFLCircleSerchPoint.h"
#include "Player/Character/BOCharacter.h"
#include "UObject/ConstructorHelpers.h"

UMonsterSpawn::UMonsterSpawn()
{
	static ConstructorHelpers::FClassFinder<AMonsterCharacter> MonsterBP(
		TEXT("/Game/Blueprints/Monster/MonsterCharcter/BP_MonsterCharacter.BP_MonsterCharacter.BP_MonsterCharacter_C"));

	if (MonsterBP.Succeeded())
	{
		MonsterClass = MonsterBP.Class;
	}
}

void UMonsterSpawn::MonsterSpawn(FVector Location, FName ID)
{
	/*
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	FNavLocation NavLocation;

	FVector MoveLocation;

	float Radius = 200;


	if (NavSystem && !NavSystem->ProjectPointToNavigation(Location, NavLocation))
	{

		for (int i = 0; i < 144; ++i)
		{
			float Angle = FMath::DegreesToRadians(i * 2.5f);

			FVector Point = Location;

			Point.X += FMath::Cos(Angle) * Radius;
			Point.Y += FMath::Sin(Angle) * Radius;

			// Point = Center에서 정확히 Radius만큼 떨어진 위치
			if (NavSystem->ProjectPointToNavigation(Point, NavLocation, FVector(200, 200, 2000.0f)))
			{

				AMonsterCharacter* TestMonster = NewObject<AMonsterCharacter>(this);

				MoveLocation = NavLocation.Location;

				if (ABOCharacter* Player = Cast<ABOCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
				{
					UNavigationPath* Path =
						UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(),
																			 Player->GetActorLocation(),
																			 MoveLocation,
																			 TestMonster);
					if (Path && Path->IsValid())
					{
						if (!Path->IsPartial())
						{
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		MoveLocation = Location;
	}
	*/

	FActorSpawnParameters SpawnParams;

	SpawnParams.CustomPreSpawnInitalization = [ID](AActor* SpawnedActor)
	{
		AMonsterCharacter* Monster = Cast<AMonsterCharacter>(SpawnedActor);

		if (Monster)
		{
			Monster->SetMonsterID(ID);
		}
	};

	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMonsterCharacter* SpawnedActor = GetWorld()->SpawnActor<AMonsterCharacter>(MonsterClass,
																				Location,
																				FRotator::ZeroRotator,
																				SpawnParams);
}
