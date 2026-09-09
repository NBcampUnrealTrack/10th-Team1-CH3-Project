#include "Items/Actors/ItemPickupBase.h"

AItemPickupBase::AItemPickupBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
