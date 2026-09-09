#include "Projectiles/ProjectileBase.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}
