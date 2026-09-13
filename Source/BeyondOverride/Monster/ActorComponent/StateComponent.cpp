// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/StateComponent.h"

UStateComponent::UStateComponent()
{
	bCanPatrol = true;
}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnPoint = GetOwner()->GetActorLocation();
}

FVector UStateComponent::GetSpawnPoint() const
{
	return SpawnPoint;
}

bool UStateComponent::GetBeCanPatrol() const
{
	return bCanPatrol;
}

bool UStateComponent::GetContinueTargeting() const
{
	return ContinueTargeting;
}

void UStateComponent::TrueBeCanPatrol()
{
	bCanPatrol = true;
}

void UStateComponent::FalseBeCanPatrol()
{
	bCanPatrol = false;
}

void UStateComponent::TrueContinueTargeting()
{
	ContinueTargeting = true;
}

void UStateComponent::FalseContinueTargeting()
{
	ContinueTargeting = false;
}

void UStateComponent::CallPatrolTimer()
{
	float RangeRand = 4.0f;
	float TimerSec = 8.0f + FMath::RandRange(-RangeRand, RangeRand);

	GetWorld()->GetTimerManager().SetTimer(PatrolTimer,
										   this,
										   &UStateComponent::TrueBeCanPatrol,
										   TimerSec,
										   false);
}

void UStateComponent::CallContinueTimer()
{
	GetWorld()->GetTimerManager().SetTimer(ContinueTimer,
										   this,
										   &UStateComponent::FalseContinueTargeting,
										   15.0f,
										   false);
}

void UStateComponent::ReCallContinueTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ContinueTimer);
	GetWorld()->GetTimerManager().SetTimer(ContinueTimer,
										   this,
										   &UStateComponent::FalseContinueTargeting,
										   15.0f,
										   false);
}
