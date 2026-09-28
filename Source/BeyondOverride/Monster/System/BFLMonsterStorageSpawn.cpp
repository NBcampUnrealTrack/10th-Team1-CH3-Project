// 26/09/28 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLMonsterStorageSpawn.h"

// Add include
#include "DataTables/Monster/MonsterStorageInfo.h"
#include "Monster/MonsterRootBox/MonsterStorageContainerActor.h"

UBFLMonsterStorageSpawn::UBFLMonsterStorageSpawn()
{
}

void UBFLMonsterStorageSpawn::StorageSpawn(FVector SpawnLocation,
										   FName StorageTarget,
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

	UBlueprint* StorageBlueprint = LoadObject<UBlueprint>(
		nullptr,
		TEXT("/Game/Blueprints/Monster/MonsterContainer/BP_MonsterStorageContainerActor.BP_MonsterStorageContainerActor"));

	if (!StorageBlueprint)
	{
		return;
	}

	UClass* StorageClass = StorageBlueprint->GeneratedClass;

	if (!StorageClass || !StorageClass->IsChildOf(AMonsterStorageContainerActor::StaticClass()))
	{
		return;
	}

	UDataTable* StorageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_MonsterStorageInfo.DT_MonsterStorageInfo"));
	if (!StorageDataTable)
	{
		return;
	}

	FMonsterStorageInfo* StorageData = StorageDataTable->FindRow<FMonsterStorageInfo>(StorageTarget, TEXT("Storage"));
	if (!StorageData)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;

	SpawnParams.CustomPreSpawnInitalization = [StorageData, StorageTarget](AActor* SpawnedActor)
	{
		AMonsterStorageContainerActor* Storage = Cast<AMonsterStorageContainerActor>(SpawnedActor);

		if (Storage)
		{
			Storage->MeshInfoSetUp(StorageTarget,
								   StorageData->SkeletalScale,
								   StorageData->SkeletalLocation,
								   StorageData->SkeletalRotation,
								   StorageData->StorageSkeletal,
								   StorageData->StorageAnimInstance);
		}
	};

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMonsterStorageContainerActor* SpawnedActor = World->SpawnActor<AMonsterStorageContainerActor>(StorageClass,
																								   SpawnLocation,
																								   FRotator::ZeroRotator,
																								   SpawnParams);
}
