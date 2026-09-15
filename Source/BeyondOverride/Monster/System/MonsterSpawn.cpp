
// 26/09/15 Copyright Jinho Song

// Base include
#include "Monster/System/MonsterSpawn.h"

// Add include
#include "MonsterSpawn.h"

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"

void UMonsterSpawn::MonsterSpawn(TSubclassOf<AMonsterCharacter> MonsterClass, FVector Location, FName ID)
{

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
																				Location,
																				FRotator::ZeroRotator,
																				SpawnParams);
}
