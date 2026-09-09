#include "Subsystems/ItemDataSubsystem.h"

#include "DataAssets/ItemDataRegistry.h"

void UItemDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftObjectPath RegistryPath(TEXT("/Game/DataAssets/DA_ItemDataRegistry.DA_ItemDataRegistry"));
	ItemDataRegistry = Cast<UItemDataRegistry>(RegistryPath.TryLoad());
}

void UItemDataSubsystem::Deinitialize()
{
	ItemDataRegistry = nullptr;

	Super::Deinitialize();
}
