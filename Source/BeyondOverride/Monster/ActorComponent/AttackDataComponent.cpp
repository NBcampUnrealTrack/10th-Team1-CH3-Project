// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/AttackDataComponent.h"

UAttackDataComponent::UAttackDataComponent()
{
}

void UAttackDataComponent::SetTargetLocation(FVector Point)
{
	TargetLocation = Point;
}

FVector UAttackDataComponent::GetTargetLocation() const
{
	return TargetLocation;
}

int32 UAttackDataComponent::GetAttackDamage() const
{
	return AttackDamage;
}

int32 UAttackDataComponent::GetRapidCount() const
{
	return RapidCount;
}

float UAttackDataComponent::GetAttackSpeed() const
{
	return AttackSpeed;
}

float UAttackDataComponent::GetAttackRange() const
{
	return AttackRange;
}

bool UAttackDataComponent::IsDelay() const
{
	return AttackHold;
}

void UAttackDataComponent::EndAttackHold()
{
	AttackHold = false;
}

void UAttackDataComponent::CallAttackDelay()
{
	if (AttackHold)
	{
		return;
	}

	AttackHold = true;
	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandler,
										   this,
										   &UAttackDataComponent::EndAttackHold,
										   AttackSpeed,
										   false);
}

void UAttackDataComponent::BeginPlay()
{
	Super::BeginPlay();
}
