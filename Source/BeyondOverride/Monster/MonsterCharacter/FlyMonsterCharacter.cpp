// 26/09/20 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/MonsterCharacter/FlyMonsterCharacter.h"

// Add include
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AFlyMonsterCharacter::AFlyMonsterCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->GravityScale = 0.0f;
		Movement->SetMovementMode(MOVE_Flying);
		Movement->DefaultLandMovementMode = MOVE_Flying;
		Movement->SetPlaneConstraintEnabled(false);
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = FRotator(540.0f, 540.0f, 0.0f);
	}
}

void AFlyMonsterCharacter::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	if (Paths.IsEmpty())
	{

		FVector TargetLocation = FVector(FMath::RandRange(-5000.0f, 5000.0f),
										 FMath::RandRange(-5000.0f, 5000.0f),
										 FMath::RandRange(100.0f, 500.0f));

		uint32 loop = 0;

		Paths.Add(GetActorLocation());

		while (loop < 50 && (Paths.IsEmpty() || *Paths.rbegin() != TargetLocation))
		{
			Paths.Append(TestNav(TargetLocation, *Paths.rbegin()));
			loop = loop + 1;
		}

		if (loop >= 50)
		{
			Paths.Empty();
			loop = 0;
		}

		if (Paths.IsEmpty())
		{
			return;
		}
	}

	MoveFlying(*Paths.begin());

	if (FVector::PointsAreNear(GetActorLocation(), *Paths.begin(), 500.0f))
	{
		Paths.RemoveAt(0);
	}
}

void AFlyMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// 자료 저장은 2개 장애물 없을 시 Start부터 End까지 반환
// 장애물 있을 시 우회 시작부터 끝까지 반환
// 우회 중 장애물 충돌시 이동 가능 지점을 End로 반환
TArray<FVector> AFlyMonsterCharacter::TestNav(const FVector& TargetLocation, const FVector& StartLocation)
{

	TArray<FVector> Path;

	float Radius;
	float HalfHeight;
	GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);

	float TraceRadius = Radius > HalfHeight ? Radius : HalfHeight;

	FVector Start = StartLocation;
	FVector End = TargetLocation;

	Path.Add(End);

	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(this,
														 Start,
														 End,
														 TraceRadius * 1.4, // Radius
														 TraceRadius * 1.4, // Half Height
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
		FVector WallBoundsOrigin = WallBounds.Origin;

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

			FVector WallEndFirst = GetWallEndPoint(WallSideDirection1 * loop,
												   HitResult.ImpactPoint,
												   WallBoundsOrigin,
												   WallBoundsExtent,
												   WallRotation,
												   EndOne);

			FVector WallEndSecond = GetWallEndPoint(WallSideDirection2 * loop,
													HitResult.ImpactPoint,
													WallBoundsOrigin,
													WallBoundsExtent,
													WallRotation,
													EndTwo);

			WallEndFirst = WallEndFirst + (WallSideDirection1 * loop) * (TraceRadius * 1.4 / 2);
			WallEndSecond = WallEndSecond + (WallSideDirection2 * loop) * (TraceRadius * 1.4 / 2);

			TArray<FVector> WallEndCheck;
			WallEndCheck.Add(WallEndFirst);
			WallEndCheck.Add(WallEndSecond);

			TArray<float> WallEndCheckDistance;
			WallEndCheckDistance.Add(EndOne);
			WallEndCheckDistance.Add(EndTwo);

			FHitResult WallCheckHitResult1;
			FHitResult WallCheckHitResult2;

			for (int Check = 0; Check < 2; Check = Check + 1)
			{
				bool Trace1 = UKismetSystemLibrary::CapsuleTraceSingle(this,
																	   Start,
																	   WallEndCheck[Check],
																	   TraceRadius * 1.4, // Radius
																	   TraceRadius * 1.4, // Half Height
																	   UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
																	   false,             // Complex
																	   TArray<AActor*>(), // Ignore Actors
																	   EDrawDebugTrace::None,
																	   WallCheckHitResult1,
																	   true); // Ignore Self

				bool Trace2 = UKismetSystemLibrary::CapsuleTraceSingle(this,
																	   HitResult.ImpactPoint,
																	   WallEndCheck[Check],
																	   TraceRadius * 1.4, // Radius
																	   TraceRadius * 1.4, // Half Height
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
				}
				CanMoveEndPoint.Add(WallEndCheck[Check]);
				EndDistance.Add(WallEndCheckDistance[Check]);
			}
		}

		float WallDepth;

		FVector WallEndPoint = GetWallEndPoint(HitResult.ImpactNormal,
											   HitResult.ImpactPoint,
											   WallBoundsOrigin,
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
			TracePointCheck = TracePointCheck + ImpactDirection * (TraceRadius * 1.4 / 2);

			bool Trace = UKismetSystemLibrary::CapsuleTraceSingle(this,
																  CanMoveEndPoint[loop],
																  TracePointCheck,
																  TraceRadius * 1.4, // Radius
																  TraceRadius * 1.4, // Half Height
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

void AFlyMonsterCharacter::MoveFlying(const FVector& TargetLocation)
{
	const FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction);
}

FVector AFlyMonsterCharacter::GetWallEndPoint(FVector DirectionData,
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
		float XBoundary = LocalDirection.X > 0.0f
							  ? TargetExtent.X
							  : -TargetExtent.X;

		TX =
			(XBoundary - LocalImpact.X) / LocalDirection.X;
	}

	if (!FMath::IsNearlyZero(LocalDirection.Y))
	{
		float YBoundary = LocalDirection.Y > 0.0f
							  ? TargetExtent.Y
							  : -TargetExtent.Y;

		TY =
			(YBoundary - LocalImpact.Y) / LocalDirection.Y;
	}

	if (!FMath::IsNearlyZero(LocalDirection.Z))
	{
		float ZBoundary = LocalDirection.Z > 0.0f
							  ? TargetExtent.Z
							  : -TargetExtent.Z;

		TZ =
			(ZBoundary - LocalImpact.Z) / LocalDirection.Z;
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

	FVector WorldEdge = TargetOrigin +
						TargetRotation.RotateVector(LocalEdge);

	Distance = FVector::Distance(ImpactData, WorldEdge);

	return WorldEdge;
}
