// 26/09/19 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLCircleSerchPoint.h"

// Add include
#include "NavigationSystem.h"

FVector UBFLCircleSerchPoint::CircleSerch(bool CanLook, bool NotReturn, AActor* Target, FSerchValues Values, UObject* WorldContextObject)
{

	if (!IsValid(WorldContextObject))
	{
		return FVector::ZeroVector;
	}

	UWorld* World = WorldContextObject->GetWorld();

	if (!World)
	{
		return FVector::ZeroVector;
	}

	// BasicValue
	float Angle;
	float SampleAngle = 360.0f / Values.Smaple;

	FVector FindPoint = FVector::ZeroVector;
	FVector SamplePoint = FVector::ZeroVector;

	FNavLocation NavLocation;

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

	// TraceValue
	FVector EndTrace = Values.Centor;
	FVector StartTrace = SamplePoint;

	FHitResult TraceHit;

	FCollisionQueryParams QueryParams;
	FCollisionObjectQueryParams TraceQueryParams;

	for (size_t i = 0; i < Values.Smaple; ++i)
	{
		Angle = FMath::DegreesToRadians(Values.BaseAngle + (i * SampleAngle));

		FVector Point = Values.Centor;

		Point.X += FMath::Cos(Angle) * Values.Radius;
		Point.Y += FMath::Sin(Angle) * Values.Radius;

		// Point = Center에서 정확히 Radius만큼 떨어진 위치
		if (NavSystem->ProjectPointToNavigation(Point, NavLocation, FVector(Values.XYRange, Values.XYRange, Values.ZRange)))
		{
			SamplePoint = NavLocation.Location;

			if (!CanLook)
			{
				FindPoint = SamplePoint;
				return FindPoint;
			}

			TraceQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
			TraceQueryParams.AddObjectTypesToQuery(ECC_Pawn);

			bool isTraceHit = World->LineTraceSingleByObjectType(TraceHit,
																 StartTrace,
																 EndTrace,
																 TraceQueryParams,
																 QueryParams);

			if (!Target && !isTraceHit)
			{
				FindPoint = SamplePoint;
				return FindPoint;
			}

			if (isTraceHit &&
				IsValid(TraceHit.GetActor()) &&
				TraceHit.GetActor() == Target)
			{
				FindPoint = SamplePoint;
				return FindPoint;
			}
		}
	}
	if (NotReturn)
	{
		return FindPoint;
	}
	else
	{
		FindPoint = SamplePoint;
		return FindPoint;
	}
}
