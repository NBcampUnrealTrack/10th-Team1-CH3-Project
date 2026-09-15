// 26/09/14 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

// UHT Header
#include "BalisticTrace.generated.h"

// DELEGATE
DECLARE_MULTICAST_DELEGATE(FOnBalisticHit);

UCLASS()
class BEYONDOVERRIDE_API UBalisticTrace : public UObject
{
	GENERATED_BODY()

	// Methtods
  public:
	void BalisticStart(FHitResult& Result, const AActor*& Caller, const FVector& Location, const FVector& Direction, float Delay, float Speed);

	void BalisticContinue();

	// Properties
  public:
	FOnBalisticHit OnBalisticHit;

  protected:
	bool bHit;

	float AttackDelay;
	float BulletSpeed;
	const float FlyTime = 0.1f;

	uint32 EndCount;

	FVector BulletLocation;
	FVector BulletDirection;

	FVector StartLocation = FVector::ZeroVector;
	FVector EndLocation = FVector::ZeroVector;

	const FVector Gravity = FVector(0.0f, 0.0f, -980.0f);

	FHitResult* HitResult;

	FTimerHandle Update;

	FCollisionQueryParams QueryParams;
	FCollisionObjectQueryParams TraceParams;
};
