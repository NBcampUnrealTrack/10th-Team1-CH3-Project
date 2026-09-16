// 26/09/15 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/SenseComponent.h"

USenseComponent::USenseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USenseComponent::SetTarget(ABOCharacter* Target)
{
	MonsterTarget = Target;
}

ABOCharacter* USenseComponent::GetTarget() const
{
	return MonsterTarget;
}

void USenseComponent::SetTargetPoint(FVector Point)
{
	TargetPoint = Point;
}

FVector USenseComponent::GetTargetPoint() const
{
	return TargetPoint;
}

void USenseComponent::SetSpawnPoint(FVector Point)
{
	SpawnPoint = Point;
}

FVector USenseComponent::GetSpawnPoint() const
{
	return SpawnPoint;
}

void USenseComponent::BeginPlay()
{
	Super::BeginPlay();
}
