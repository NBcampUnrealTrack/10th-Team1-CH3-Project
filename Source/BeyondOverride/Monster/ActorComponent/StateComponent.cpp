// 26/09/10 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/StateComponent.h"

// Add include
#include "Player/Character/BOCharacter.h"

UStateComponent::UStateComponent()
{
	bCanPatrol = true;
	LocationPatrolActor = nullptr;
	NowTarget = nullptr;
	LocationPatrolPoint = FVector::ZeroVector;
}

void UStateComponent::SetIsCallLocation(bool value)
{
	IsCallLocation = value;
}

bool UStateComponent::GetIsCallLocation() const
{
	return IsCallLocation;
}

void UStateComponent::SetTarget(ABOCharacter* Character)
{
	NowTarget = Character;
}

ABOCharacter* UStateComponent::GetTarget() const
{
	return NowTarget;
}

void UStateComponent::SetLocationPatrolActor(AActor* PlayActor)
{
	LocationPatrolActor = PlayActor;
}

AActor* UStateComponent::GetLocationPatrolActor() const
{
	return LocationPatrolActor;
}

void UStateComponent::SetLocationPatrolPoint(FVector Location)
{
	LocationPatrolPoint = Location;
}

FVector UStateComponent::GetLocationPatrolPoint() const
{
	return LocationPatrolPoint;
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
										   30.0f,
										   false);
}

void UStateComponent::ReCallContinueTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ContinueTimer);
	GetWorld()->GetTimerManager().SetTimer(ContinueTimer,
										   this,
										   &UStateComponent::FalseContinueTargeting,
										   30.0f,
										   false);
}

bool UStateComponent::IsLocation() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(LocationPatrolTimer);
}

void UStateComponent::CallLocationPatrolTimer()
{
	GetWorld()->GetTimerManager().SetTimer(LocationPatrolTimer,
										   FTimerDelegate(),
										   10.0f,
										   false);
}

bool UStateComponent::IsHearing() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(HearingTimer);
}

void UStateComponent::CallHearingTimer()
{
	if (IsHearing())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(HearingTimer,
										   FTimerDelegate(),
										   1.0f,
										   false);
}

bool UStateComponent::IsGetDamage() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(GetDamageTimer);
}

void UStateComponent::CallGetDamage()
{
	if (IsGetDamage())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(GetDamageTimer,
										   FTimerDelegate(),
										   0.02f,
										   false);
}

bool UStateComponent::IsCalling() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(CallingTimer);
}

void UStateComponent::SetCallingTimer()
{
	if (IsGetDamage())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(CallingTimer,
										   FTimerDelegate(),
										   0.02f,
										   false);
}

bool UStateComponent::IsSttandOff() const
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	return TimerManager.IsTimerActive(SttandOffTimer);
}

void UStateComponent::SetSttandOffTimer()
{
	if (IsSttandOff())
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(SttandOffTimer,
										   FTimerDelegate(),
										   10.0f,
										   false);
}
