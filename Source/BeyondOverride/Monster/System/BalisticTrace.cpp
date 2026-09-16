// 26/09/14 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BalisticTrace.h"

void UBalisticTrace::BalisticStart(const AActor* Caller, const FVector& Location, const FVector& Direction, float Delay, float Speed)
{

	TraceParams.AddObjectTypesToQuery(ECC_Pawn);
	TraceParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddIgnoredActor(Caller);

	HitResult;
	AttackDelay = Delay;
	BulletSpeed = Speed;
	BulletLocation = Location;
	BulletDirection = Direction;

	StartLocation = BulletLocation;

	FVector InitialVelocity = BulletDirection * BulletSpeed;

	EndLocation = StartLocation + InitialVelocity * FlyTime + 0.5f * Gravity * FlyTime * FlyTime;
	EndCount = EndCount + 1;

	bHit = GetWorld()->LineTraceSingleByObjectType(HitResult,
												   StartLocation,
												   EndLocation,
												   TraceParams,
												   QueryParams);

	GetWorld()->GetTimerManager().SetTimer(Update,
										   this,
										   &UBalisticTrace::BalisticContinue,
										   FlyTime,
										   false);
}

void UBalisticTrace::BalisticContinue()
{

	if (bHit || EndCount * FlyTime >= AttackDelay)
	{

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();

			if (IsValid(HitActor))
			{
				OnBalisticHit.Broadcast(HitActor);
			}
		}

		BulletLocation = FVector::ZeroVector;
		BulletDirection = FVector::ZeroVector;
		StartLocation = FVector::ZeroVector;
		EndLocation = FVector::ZeroVector;
		EndCount = 0;
		bHit = false;
		return;
	}
	StartLocation = EndLocation;

	FVector InitialVelocity = BulletDirection * BulletSpeed;

	EndLocation = StartLocation + InitialVelocity * FlyTime + 0.5f * Gravity * FlyTime * FlyTime;
	EndCount = EndCount + 1;

	bHit = GetWorld()->LineTraceSingleByObjectType(HitResult,
												   StartLocation,
												   EndLocation,
												   TraceParams,
												   QueryParams);

	GetWorld()->GetTimerManager().SetTimer(Update,
										   this,
										   &UBalisticTrace::BalisticContinue,
										   FlyTime,
										   false);
}
