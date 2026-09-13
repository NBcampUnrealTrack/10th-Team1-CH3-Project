// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/AttackDataComponent.h"

UAttackDataComponent::UAttackDataComponent()
{
}

void UAttackDataComponent::SetProtect(int32 GetProtect)
{
	Protect = GetProtect;
}

void UAttackDataComponent::SetAttackDamage(int32 Damage)
{
	AttackDamage = Damage;
}

void UAttackDataComponent::SetRapidCount(int32 Rapid)
{
	RapidCount = Rapid;
}

void UAttackDataComponent::SetAttackDelay(float Speed)
{
	AttackDelay = Speed;
}

void UAttackDataComponent::SetAttackRange(float Range)
{
	AttackRange = Range;
}

void UAttackDataComponent::SetTargetLocation(FVector Point)
{
	TargetLocation = Point;
}

int32 UAttackDataComponent::GetProtect() const
{
	return Protect;
}

int32 UAttackDataComponent::GetAttackDamage() const
{
	return AttackDamage;
}

int32 UAttackDataComponent::GetRapidCount() const
{
	return RapidCount;
}

float UAttackDataComponent::GetAttackDelay() const
{
	return AttackDelay;
}

float UAttackDataComponent::GetAttackRange() const
{
	return AttackRange;
}

FVector UAttackDataComponent::GetTargetLocation() const
{
	return TargetLocation;
}

bool UAttackDataComponent::IsDelay() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(AttackDelayHandler);
}

void UAttackDataComponent::CallAttackDelay()
{
	if (IsDelay())
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(AttackDelayHandler,
										   FTimerDelegate(),
										   AttackDelay,
										   false);
}

void UAttackDataComponent::BeginPlay()
{
	Super::BeginPlay();
}
