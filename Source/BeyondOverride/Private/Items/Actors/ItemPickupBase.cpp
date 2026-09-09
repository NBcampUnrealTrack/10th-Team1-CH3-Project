#include "Items/Actors/ItemPickupBase.h"

AItemPickupBase::AItemPickupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComp);

	ItemInstance = nullptr;
}

UItemInstanceBase* AItemPickupBase::GetItemInstance() const
{
	return ItemInstance;
}

void AItemPickupBase::BeginPlay()
{
	Super::BeginPlay();
}
