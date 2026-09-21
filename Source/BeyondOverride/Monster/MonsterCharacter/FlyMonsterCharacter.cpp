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
		FVector TargetLocation = FVector(0,
										 0,
										 3000.0f);

		// FVector TargetLocation = FVector(FMath::RandRange(-5000.0f, 5000.0f),
		//									FMath::RandRange(-5000.0f, 5000.0f),
		//									FMath::RandRange(100.0f, 500.0f));

		uint32 loop = 0;

		Paths.Add(GetActorLocation());

		while (loop < 500 && (Paths.IsEmpty() || *Paths.rbegin() != TargetLocation))
		{
			Paths.Append(TestNav(TargetLocation, *Paths.rbegin()));
			loop = loop + 1;
		}

		if (loop >= 500)
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
		FVector WallExtent = WallCollision->Bounds.BoxExtent;
		FVector WallLocation = HitActor->GetActorLocation();
		FRotator WallRotation = HitActor->GetActorRotation();

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

		// 우회 후보지
		FVector WallEndPoint1 = GetWallEndPoint(HitResult.ImpactPoint,
												WallSideDirection1,
												WallLocation,
												WallExtent,
												WallRotation);

		FVector WallEndPoint2 = GetWallEndPoint(HitResult.ImpactPoint,
												-WallSideDirection1,
												WallLocation,
												WallExtent,
												WallRotation);

		FVector WallEndPoint3 = GetWallEndPoint(HitResult.ImpactPoint,
												WallSideDirection2,
												WallLocation,
												WallExtent,
												WallRotation);

		FVector WallEndPoint4 = GetWallEndPoint(HitResult.ImpactPoint,
												-WallSideDirection2,
												WallLocation,
												WallExtent,
												WallRotation);

		WallEndPoint1 = WallEndPoint1 + WallSideDirection1 * (TraceRadius * 1.4 / 2);
		WallEndPoint2 = WallEndPoint2 - WallSideDirection1 * (TraceRadius * 1.4 / 2);
		WallEndPoint3 = WallEndPoint3 + WallSideDirection2 * (TraceRadius * 1.4 / 2);
		WallEndPoint4 = WallEndPoint4 - WallSideDirection2 * (TraceRadius * 1.4 / 2);

		FVector ImpactWallDirection = (HitResult.ImpactPoint - WallLocation).GetSafeNormal();

		FVector WallAxisX = WallRotation.RotateVector(FVector::ForwardVector);
		FVector WallAxisY = WallRotation.RotateVector(FVector::RightVector);
		FVector WallAxisZ = WallRotation.RotateVector(FVector::UpVector);

		float BuildingLength = 2.0f * (FMath::Abs(FVector::DotProduct(ImpactWallDirection, WallAxisX)) * WallExtent.X +
									   FMath::Abs(FVector::DotProduct(ImpactWallDirection, WallAxisY)) * WallExtent.Y +
									   FMath::Abs(FVector::DotProduct(ImpactWallDirection, WallAxisZ)) * WallExtent.Z);

		FVector WallTracePoint1 = WallEndPoint1 + ImpactWallDirection * BuildingLength;
		FVector WallTracePoint2 = WallEndPoint2 + ImpactWallDirection * BuildingLength;
		FVector WallTracePoint3 = WallEndPoint3 + ImpactWallDirection * BuildingLength;
		FVector WallTracePoint4 = WallEndPoint4 + ImpactWallDirection * BuildingLength;

		WallTracePoint1 = WallTracePoint1 + ImpactWallDirection * (TraceRadius * 1.4 / 2);
		WallTracePoint2 = WallTracePoint2 + ImpactWallDirection * (TraceRadius * 1.4 / 2);
		WallTracePoint3 = WallTracePoint3 + ImpactWallDirection * (TraceRadius * 1.4 / 2);
		WallTracePoint4 = WallTracePoint4 + ImpactWallDirection * (TraceRadius * 1.4 / 2);

		FHitResult WallHitResult1;
		FHitResult WallHitResult2;
		FHitResult WallHitResult3;
		FHitResult WallHitResult4;

		bool bHit1 = UKismetSystemLibrary::CapsuleTraceSingle(this,
															  WallEndPoint1,
															  WallTracePoint1,
															  TraceRadius * 1.4, // Radius
															  TraceRadius * 1.4, // Half Height
															  UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
															  false,             // Complex
															  TArray<AActor*>(), // Ignore Actors
															  EDrawDebugTrace::None,
															  WallHitResult1,
															  true); // Ignore Self

		bool bHit2 = UKismetSystemLibrary::CapsuleTraceSingle(this,
															  WallEndPoint2,
															  WallTracePoint2,
															  TraceRadius * 1.4, // Radius
															  TraceRadius * 1.4, // Half Height
															  UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
															  false,             // Complex
															  TArray<AActor*>(), // Ignore Actors
															  EDrawDebugTrace::None,
															  WallHitResult2,
															  true); // Ignore Self

		bool bHit3 = UKismetSystemLibrary::CapsuleTraceSingle(this,
															  WallEndPoint3,
															  WallTracePoint3,
															  TraceRadius * 1.4, // Radius
															  TraceRadius * 1.4, // Half Height
															  UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
															  false,             // Complex
															  TArray<AActor*>(), // Ignore Actors
															  EDrawDebugTrace::None,
															  WallHitResult3,
															  true); // Ignore Self

		bool bHit4 = UKismetSystemLibrary::CapsuleTraceSingle(this,
															  WallEndPoint4,
															  WallTracePoint4,
															  TraceRadius * 1.4, // Radius
															  TraceRadius * 1.4, // Half Height
															  UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
															  false,             // Complex
															  TArray<AActor*>(), // Ignore Actors
															  EDrawDebugTrace::None,
															  WallHitResult4,
															  true); // Ignore Self

		if (!bHit1)
		{
			Path.Add(WallEndPoint1);
			Path.Add(WallTracePoint1);
			return Path;
		}
		if (!bHit2)
		{
			Path.Add(WallEndPoint2);
			Path.Add(WallTracePoint2);
			return Path;
		}
		if (!bHit3)
		{
			Path.Add(WallEndPoint3);
			Path.Add(WallTracePoint3);
			return Path;
		}
		if (!bHit4)
		{
			Path.Add(WallEndPoint4);
			Path.Add(WallTracePoint4);
			return Path;
		}
	}

	return Path;
}

void AFlyMonsterCharacter::MoveFlying(const FVector& TargetLocation)
{
	const FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	AddMovementInput(Direction);
}

FVector AFlyMonsterCharacter::GetWallEndPoint(const FVector& Start,
											  const FVector& Direction,
											  const FVector& WallLocation,
											  const FVector& WallExtent,
											  const FRotator& WallRotation)
{
	// 월드 좌표를 벽의 로컬 좌표로 변환
	FVector LocalStart = WallRotation.UnrotateVector(Start - WallLocation);

	// 월드 방향을 벽의 로컬 방향으로 변환
	FVector LocalDirection = WallRotation.UnrotateVector(Direction);

	float Distance = FLT_MAX;

	if (!FMath::IsNearlyZero(LocalDirection.X))
	{
		float BoundaryX =
			LocalDirection.X > 0.0f ? WallExtent.X : -WallExtent.X;

		float T =
			(BoundaryX - LocalStart.X) / LocalDirection.X;

		if (T >= 0.0f)
		{
			Distance = FMath::Min(Distance, T);
		}
	}

	if (!FMath::IsNearlyZero(LocalDirection.Y))
	{
		float BoundaryY =
			LocalDirection.Y > 0.0f ? WallExtent.Y : -WallExtent.Y;

		float T =
			(BoundaryY - LocalStart.Y) / LocalDirection.Y;

		if (T >= 0.0f)
		{
			Distance = FMath::Min(Distance, T);
		}
	}

	if (!FMath::IsNearlyZero(LocalDirection.Z))
	{
		float BoundaryZ =
			LocalDirection.Z > 0.0f ? WallExtent.Z : -WallExtent.Z;

		float T =
			(BoundaryZ - LocalStart.Z) / LocalDirection.Z;

		if (T >= 0.0f)
		{
			Distance = FMath::Min(Distance, T);
		}
	}

	UE_LOG(LogTemp, Warning,
		   TEXT("WallEndPoint Distance: %f | Start: %s | Direction: %s"),
		   Distance,
		   *Start.ToString(),
		   *Direction.ToString());

	return Start + Direction * Distance;
}
