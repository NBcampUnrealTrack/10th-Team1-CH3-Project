// 26/09/28 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLMonsterStorageSpawn.h"

// Engine include
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// DataTable include
#include "DataTables/Monster/MonsterStorageInfo.h"

// SpawnVolume include
#include "GameFlow/Spawn/SpawnVolume.h"

// Monster Storage include
#include "Monster/MonsterRootBox/MonsterStorageContainerActor.h"

// Container Data include
#include "DataTables/Farming/SpawnData.h"

// Item include
#include "Factory/ItemFactory.h"
#include "Items/Objects/ItemInstanceBase.h"

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

	ASpawnVolume* TargetSpawnVolume = FindSpawnVolume(SpawnLocation, World);
	FName RegionID;
	if (TargetSpawnVolume)
	{
		RegionID = TargetSpawnVolume->GetRegionID();
	}

	UDataTable* ContainerDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTables/DT_ContainerData.DT_ContainerData"));
	if (!ContainerDataTable)
	{
		return;
	}

	int32 SpawnCount = FMath::RandRange(1.0f, 10.0f);

	TMap<FName, int32> ItemCounts;

	TArray<UItemInstanceBase*> Items;

	FContainerData* ContainerData = ContainerDataTable->FindRow<FContainerData>(RegionID, TEXT("MonsterStorage"));
	if (ContainerData)
	{
		if (ContainerData->SpawnEntries.Num() > 0)
		{
			for (int32 Index = 0; Index < SpawnCount; ++Index)
			{
				float RandomValue = FMath::FRand();
				float AccumulatedProbability = 0.0f;

				FName SelectedItemID = NAME_None;

				for (const FSpawnEntry& SpawnEntry : ContainerData->SpawnEntries)
				{
					AccumulatedProbability += SpawnEntry.Prob;

					if (RandomValue <= AccumulatedProbability)
					{
						SelectedItemID = SpawnEntry.ID;
						break;
					}
				}

				if (SelectedItemID.IsNone())
				{
					continue;
				}

				ItemCounts.FindOrAdd(SelectedItemID)++;
			}
		}

		for (const TPair<FName, int32>& ItemCount : ItemCounts)
		{
			for (int32 Index = 0; Index < ItemCount.Value; ++Index)
			{
				UItemInstanceBase* Item =
					FItemFactory::CreateItemInstance(
						WorldContextObject,
						ItemCount.Key);

				if (!IsValid(Item))
				{
					continue;
				}

				Items.Add(Item);
			}
		}
	}

	FRotator SpawnRotate = FRotator::ZeroRotator;

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
																								   SpawnRotate,
																								   SpawnParams);

	if (!SpawnedActor)
	{
		return;
	}

	if (!Items.IsEmpty())
	{
		SpawnedActor->SetItems(Items);
	}
}

ASpawnVolume* UBFLMonsterStorageSpawn::FindSpawnVolume(FVector SpawnLocation,
													   UWorld* World)
{
	TArray<AActor*> SpawnVolumes;

	UGameplayStatics::GetAllActorsOfClass(World,
										  ASpawnVolume::StaticClass(),
										  SpawnVolumes);

	ASpawnVolume* TargetSpawnVolume = nullptr;

	for (AActor* Actor : SpawnVolumes)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(Actor);

		if (!SpawnVolume || !SpawnVolume->BoxComp)
		{
			continue;
		}

		if (SpawnVolume->BoxComp->Bounds.GetBox().IsInside(SpawnLocation))
		{
			TargetSpawnVolume = SpawnVolume;
			break;
		}
	}
	return TargetSpawnVolume;
}
