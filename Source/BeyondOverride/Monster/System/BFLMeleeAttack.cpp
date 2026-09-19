// 26/09/19 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/System/BFLMeleeAttack.h"

// Add include
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Character/BOCharacter.h"

AActor* UBFLMeleeAttack::DashAttack(ACharacter* Caster, ACharacter* Target, float AttackRange)
{
	FVector Direction = (Target->GetActorLocation() - Caster->GetActorLocation()).GetSafeNormal();
	// Caster->GetViewRotation().Vector().GetSafeNormal();

	Direction.Z = Direction.Z + 0.05;

	float DashTime = 0.3f;

	FVector LaunchVelocity = Direction * ((AttackRange * 2.3f) / DashTime);

	Caster->LaunchCharacter(LaunchVelocity,
							true,
							true);

	FHitResult HitResult;

	FVector Start = Caster->GetActorLocation();
	FVector End = Start + Direction * (AttackRange * 2.3f);

	UClass* CasterClass = Caster->GetClass();

	float Radius;
	float HalfHeight;
	Caster->GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(Caster,
														 Start,
														 End,
														 Radius,     // Radius
														 HalfHeight, // Half Height
														 UEngineTypes::ConvertToTraceType(ECC_Pawn),
														 false,             // Complex
														 TArray<AActor*>(), // Ignore Actors
														 EDrawDebugTrace::None,
														 HitResult,
														 true); // Ignore Self

	if (bHit)
	{

		AActor* HitActor = HitResult.GetActor();

		if (!IsValid(HitActor))
		{
			return nullptr;
		}
		if (HitActor->IsA(CasterClass))
		{
			TArray<AActor*> Ignores = {};
			Ignores.Add(HitActor);
			FVector RecallPoint = HitResult.ImpactPoint;
			HitActor = DashAttack(Caster, Target, AttackRange, Ignores, RecallPoint, End);
		}
		return HitActor;
	}

	return nullptr;
}

AActor* UBFLMeleeAttack::DashAttack(ACharacter* Caster, ACharacter* Target, float AttackRange, TArray<AActor*> Ignores, FVector RecallSPoint, FVector RecallEPoint)
{

	FHitResult HitResult;

	FVector Start = RecallSPoint;
	FVector End = RecallEPoint;

	UClass* CasterClass = Caster->GetClass();

	float Radius;
	float HalfHeight;
	Caster->GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingle(Caster,
														 Start,
														 End,
														 Radius,     // Radius
														 HalfHeight, // Half Height
														 UEngineTypes::ConvertToTraceType(ECC_Pawn),
														 false,   // Complex
														 Ignores, // Ignore Actors
														 EDrawDebugTrace::None,
														 HitResult,
														 true); // Ignore Self

	if (bHit)
	{

		AActor* HitActor = HitResult.GetActor();

		if (!IsValid(HitActor))
		{
			return nullptr;
		}
		if (HitActor->IsA(CasterClass))
		{
			Ignores.Add(HitActor);
			FVector RecallPoint = HitResult.ImpactPoint;
			HitActor = DashAttack(Caster, Target, AttackRange, Ignores, RecallPoint, End);
		}
		return HitActor;
	}

	return nullptr;
}
