// 26/09/23 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/ActorComponent/AirNavComponent.h"

// Add include
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAirNavComponent::UAirNavComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FVector> UAirNavComponent::AirNav(const FVector& TargetLocation, const FVector& StartLocation)
{
	TArray<FVector> Path;

	float Radius;
	float HalfHeight;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
	{
		return Path;
	}

	Owner->GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);

	float TraceRadius = Radius > HalfHeight ? Radius : HalfHeight;

	FVector Start = StartLocation;
	FVector End = TargetLocation;

	Path.Add(End);

	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(this,
														 Start,
														 End,
														 TraceRadius, // Radius
														 TraceRadius, // Half Height
														 UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
														 false,             // Complex
														 TArray<AActor*>(), // Ignore Actors
														 EDrawDebugTrace::None,
														 HitResult,
														 true); // Ignore Self

	if (bHit)
	{
		Path.Empty();

		AActor* HitActor = HitResult.GetActor();

		if (!IsValid(HitActor))
		{
			return Path;
		}

		// 장애물 우회 점을 만들기 위한 장애물의 정보
		UPrimitiveComponent* WallCollision = HitActor->FindComponentByClass<UPrimitiveComponent>();

		if (!IsValid(WallCollision))
		{
			return Path;
		}

		FBoxSphereBounds WallBounds = WallCollision->CalcLocalBounds();

		FVector WallBoundsExtent = WallBounds.BoxExtent;
		FVector WallLocalOrigin = WallBounds.Origin;

		FTransform WallTransform = WallCollision->GetComponentTransform();

		FRotator WallRotation = WallTransform.GetRotation().Rotator();

		// 이동에 대한 방향 벡터
		FVector TargetDirection = (TargetLocation - StartLocation).GetSafeNormal();

		// 충돌에 대한 방향 벡터
		FVector ImpactDirection = HitResult.ImpactNormal;

		// TargetDirection에 포함된 ImpactDirection 방향의 성분
		FVector TargetGetImpactDirection = ImpactDirection * FVector::DotProduct(TargetDirection, HitResult.ImpactNormal);

		// 벽의 법선 방향에 수직한, 벽면과 수평하게 따라가는 방향 벡터
		FVector WallSideDirection1 = (TargetDirection - TargetGetImpactDirection).GetSafeNormal();

		// 또 다른 벽의 법선 방향에 수직한, 벽면과 수평하게 따라가는 방향 벡터
		FVector WallSideDirection2 = FVector::CrossProduct(ImpactDirection, WallSideDirection1);

		TArray<bool> bGoImpactPoint = {false, false, false, false};
		TArray<float> EndDistance;
		TArray<FVector> CanMoveEndPoint;

		for (int loop = 1; loop > -2; loop = loop - 2)
		{

			float EndOne;
			float EndTwo;

			FVector LocalImpact = WallTransform.InverseTransformPosition(HitResult.ImpactPoint);

			FVector WallEndFirst = GetWallEndPoint(WallSideDirection1 * loop,
												   LocalImpact,
												   WallLocalOrigin,
												   WallBoundsExtent,
												   WallRotation,
												   EndOne);

			FVector WallEndSecond = GetWallEndPoint(WallSideDirection2 * loop,
													LocalImpact,
													WallLocalOrigin,
													WallBoundsExtent,
													WallRotation,
													EndTwo);

			WallEndFirst = WallEndFirst + (WallSideDirection1 * loop) * (TraceRadius / 2);
			WallEndSecond = WallEndSecond + (WallSideDirection2 * loop) * (TraceRadius / 2);

			TArray<FVector> WallEndCheck;
			TArray<float> WallEndCheckDistance;

			float CheckDistance = FVector::Distance(WallEndFirst, FVector(0.0f, 0.0f, 0.0f));

			if (CheckDistance < 100000.0f)
			{
				WallEndCheck.Add(WallEndFirst);
				WallEndCheckDistance.Add(EndOne);
			}

			CheckDistance = FVector::Distance(WallEndSecond, FVector(0.0f, 0.0f, 0.0f));
			if (CheckDistance < 100000.0f)
			{
				WallEndCheck.Add(WallEndSecond);
				WallEndCheckDistance.Add(EndTwo);
			}

			if (WallEndCheck.IsEmpty())
			{
				continue;
			}

			FHitResult WallCheckHitResult1;
			FHitResult WallCheckHitResult2;

			for (int Check = 0; Check < WallEndCheck.Num(); Check = Check + 1)
			{
				bool Trace1 = UKismetSystemLibrary::CapsuleTraceSingle(this,
																	   Start,
																	   WallEndCheck[Check],
																	   TraceRadius, // Radius
																	   TraceRadius, // Half Height
																	   UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
																	   false,             // Complex
																	   TArray<AActor*>(), // Ignore Actors
																	   EDrawDebugTrace::None,
																	   WallCheckHitResult1,
																	   true); // Ignore Self

				bool Trace2 = UKismetSystemLibrary::CapsuleTraceSingle(this,
																	   HitResult.ImpactPoint,
																	   WallEndCheck[Check],
																	   TraceRadius, // Radius
																	   TraceRadius, // Half Height
																	   UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
																	   false,             // Complex
																	   TArray<AActor*>(), // Ignore Actors
																	   EDrawDebugTrace::None,
																	   WallCheckHitResult2,
																	   true); // Ignore Self

				if (Trace1 && Trace2)
				{
					continue;
				}
				if (Trace2)
				{
					bGoImpactPoint[1 + Check - loop] = true;
					WallEndCheckDistance[Check] = WallEndCheckDistance[Check] +
												  FVector::Distance(HitResult.ImpactPoint, Start);
				}
				CanMoveEndPoint.Add(WallEndCheck[Check]);
				EndDistance.Add(WallEndCheckDistance[Check]);
			}
		}

		float WallDepth;

		FVector WallEndPoint = GetWallEndPoint(HitResult.ImpactNormal,
											   HitResult.ImpactPoint,
											   WallLocalOrigin,
											   WallBoundsExtent,
											   WallRotation,
											   WallDepth);

		TArray<FVector> CanMoveTracePoint;
		TArray<int32> RemoveTarget;

		for (int loop = 0; loop < CanMoveEndPoint.Num(); loop = loop + 1)
		{
			FHitResult TraceHitResult;
			FVector TracePointCheck;
			TracePointCheck = CanMoveEndPoint[loop] + ImpactDirection * WallDepth;
			TracePointCheck = TracePointCheck + ImpactDirection * (TraceRadius / 2);

			bool Trace = UKismetSystemLibrary::CapsuleTraceSingle(this,
																  CanMoveEndPoint[loop],
																  TracePointCheck,
																  TraceRadius, // Radius
																  TraceRadius, // Half Height
																  UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
																  false,             // Complex
																  TArray<AActor*>(), // Ignore Actors
																  EDrawDebugTrace::None,
																  TraceHitResult,
																  true); // Ignore Self

			if (Trace)
			{
				RemoveTarget.Add(loop);
				continue;
			}

			CanMoveTracePoint.Add(TracePointCheck);
		}
		if (!RemoveTarget.IsEmpty())
		{
			for (int index = RemoveTarget.Num() - 1; index >= 0; index = index - 1)
			{
				bGoImpactPoint.RemoveAt(RemoveTarget[index]);
				EndDistance.RemoveAt(RemoveTarget[index]);
				CanMoveEndPoint.RemoveAt(RemoveTarget[index]);
			}
		}

		float MinDistance = BIG_NUMBER;
		int32 MinIndex = INT_MAX;

		if (CanMoveTracePoint.IsEmpty())
		{
			return Path;
		}

		for (int index = 0; index < CanMoveTracePoint.Num(); index = index + 1)
		{

			if (MinDistance >= EndDistance[index])
			{
				MinDistance = EndDistance[index];
				MinIndex = index;
			}
		}
		if (bGoImpactPoint[MinIndex])
		{
			Path.Add(HitResult.ImpactPoint);
		}
		Path.Add(CanMoveEndPoint[MinIndex]);
		Path.Add(CanMoveTracePoint[MinIndex]);
	}

	return Path;
}

void UAirNavComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UAirNavComponent::GetWallEndPoint(FVector DirectionData,
										  FVector ImpactData,
										  FVector TargetOrigin,
										  FVector TargetExtent,
										  FRotator TargetRotation,
										  float& Distance)
{
	FVector LocalDirection = TargetRotation.UnrotateVector(DirectionData).GetSafeNormal();

	FVector LocalImpact = TargetRotation.UnrotateVector(ImpactData - TargetOrigin);

	FVector LocalEdge;

	float TX = BIG_NUMBER;
	float TY = BIG_NUMBER;
	float TZ = BIG_NUMBER;

	if (!FMath::IsNearlyZero(LocalDirection.X))
	{
		float XBoundary = LocalDirection.X > 0.0f ? TargetExtent.X : -TargetExtent.X;
		TX = (XBoundary - LocalImpact.X) / LocalDirection.X;
	}

	if (!FMath::IsNearlyZero(LocalDirection.Y))
	{
		float YBoundary = LocalDirection.Y > 0.0f ? TargetExtent.Y : -TargetExtent.Y;
		TY = (YBoundary - LocalImpact.Y) / LocalDirection.Y;
	}

	if (!FMath::IsNearlyZero(LocalDirection.Z))
	{
		float ZBoundary = LocalDirection.Z > 0.0f ? TargetExtent.Z : -TargetExtent.Z;
		TZ = (ZBoundary - LocalImpact.Z) / LocalDirection.Z;
	}

	float T = BIG_NUMBER;

	if (TX >= 0.0f)
	{
		T = FMath::Min(T, TX);
	}

	if (TY >= 0.0f)
	{
		T = FMath::Min(T, TY);
	}

	if (TZ >= 0.0f)
	{
		T = FMath::Min(T, TZ);
	}

	LocalEdge = LocalImpact + LocalDirection * T;

	if (FMath::Abs(LocalEdge.X) > 1000000.0f ||
		FMath::Abs(LocalEdge.Y) > 1000000.0f ||
		FMath::Abs(LocalEdge.Z) > 1000000.0f)
	{
		return FVector(100000000.0f);
	}

	FVector WorldEdge = TargetOrigin + TargetRotation.RotateVector(LocalEdge);

	Distance = FVector::Distance(ImpactData, WorldEdge);

	return WorldEdge;
}
