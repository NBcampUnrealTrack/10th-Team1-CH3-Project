// 26/09/25 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/BossCharacter/BossCharacter.h"

// Add include
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/BossController/BossAIController.h"
#include "Monster/System/BFLMeleeAttack.h"
#include "Monster/System/BFLMissileAttack.h"
#include "Monster/System/BFLSoundEvent.h"
#include "Monster/System/BalisticTrace.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/Character/BOCharacter.h"

ABossCharacter::ABossCharacter()
{
}

void ABossCharacter::MonsterAttack()
{
	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return;
	}

	ABOCharacter* Target = BossController->GetTarget();
	if (!Target)
	{
		return;
	}

	BossController->StateChange(EMonsterState::Attack, 0.3f);

	float TargetDistance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());

	int32 AttackChoice = 0;

	if (GetAttackRange() / 3 <= TargetDistance)
	{
		AttackChoice = FMath::RandRange(1.0f, 5.0f);
	}
	else if (GetAttackRange() / 2 <= TargetDistance &&
			 GetAttackRange() / 3 > TargetDistance)
	{
		AttackChoice = FMath::RandRange(1.0f, 4.0f);
	}
	else if (GetAttackRange() / 2 > TargetDistance)
	{
		AttackChoice = FMath::RandRange(0.0f, 4.0f);
	}

	if (AttackChoice <= 1)
	{

		AActor* MeleeTarget = UBFLMeleeAttack::DashAttack(this, BossController->GetTarget(), GetAttackRange() / 4);

		if (MeleeTarget)
		{

			ABOCharacter* PlayerCharacter = Cast<ABOCharacter>(MeleeTarget);
			if (!PlayerCharacter)
			{
				return;
			}
			MonsterStat->DamageLogic(MeleeTarget, MonsterStat->GetAttackDamage() * 2);
		}
		BossController->PatternHold(EBossPattern::Melee);
	}
	else if (AttackChoice > 1 &&
			 AttackChoice < 4)
	{
		MonsterStat->BalisticFire();
		UParticleSystemComponent* Particle = nullptr;

		FVector AttackPoint = GetMesh()->GetSocketLocation(*SocketName.ToString());
		FRotator AttackFocus = GetMesh()->GetSocketRotation(*SocketName.ToString());

		if (Effect)
		{
			Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
																Effect,
																AttackPoint,
																AttackFocus,
																true);
			if (Particle)
			{
				FTimerHandle DestroyParticleTimerHandle;
				TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

				GetWorld()->GetTimerManager().SetTimer(
					DestroyParticleTimerHandle,
					[WeakParticle]()
					{
						if (WeakParticle.IsValid())
						{
							WeakParticle->DestroyComponent();
						}
					},
					2.0f,
					false);
			}
		}
		BossController->PatternHold(EBossPattern::Range);
	}
	else if (AttackChoice >= 4)
	{

		MissileFire(false);

		GetWorld()->GetTimerManager().SetTimer(FireDelay,
											   this,
											   &ABossCharacter::MissileFire,
											   FMath::RandRange(0.05f, 0.07f),
											   false);

		BossController->PatternHold(EBossPattern::Missile);
	}
	MonsterStat->CallAttackLock();
}

float ABossCharacter::TakeDamage(float DamageAmount,
								 FDamageEvent const& DamageEvent,
								 AController* EventInstigator,
								 AActor* DamageCauser)
{

	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return 0.0f;
	}

	if (MonsterStat->GetCurHealth() <= MonsterStat->GetMaxHealth() / 2 &&
		BossController->CurrentPhase() == EBossPhase::Phase1)
	{
		ChangeMovement();
		HalfHealth.Broadcast();
		GetWorld()->GetTimerManager().SetTimer(Recall,
											   this,
											   &ABossCharacter::RecallSpawnPoint,
											   0.90f,
											   false);
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return ActualDamage;
}

void ABossCharacter::MissileFire(bool IsRand)
{
	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return;
	}

	ABOCharacter* Target = BossController->GetTarget();
	if (!Target)
	{
		return;
	}

	FVector StartPoint = FVector(-9700.0f, -5000.0f, 24000.0f);
	FVector EndPoint = Target->GetActorLocation();

	if (IsRand)
	{
		int32 interval = FMath::RandRange(1.0f, 16.0f) * 50;

		float RandX = FMath::RandRange(1.0f, 2.0f);
		float RandY = FMath::RandRange(1.0f, 2.0f);

		if (RandX == 1)
		{
			StartPoint.X = -(StartPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
															 interval + FMath::RandRange(-25.0f, 25.0f)));

			EndPoint.X = -(EndPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														 interval + FMath::RandRange(-25.0f, 25.0f)));
		}
		else
		{
			StartPoint.X = StartPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														   interval + FMath::RandRange(-25.0f, 25.0f));

			EndPoint.X = EndPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													   interval + FMath::RandRange(-25.0f, 25.0f));
		}

		if (RandY == 1)
		{
			StartPoint.Y = -(StartPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
															 interval + FMath::RandRange(-25.0f, 25.0f)));

			EndPoint.Y = -(EndPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														 interval + FMath::RandRange(-25.0f, 25.0f)));
		}
		else
		{
			StartPoint.Y = StartPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														   interval + FMath::RandRange(-25.0f, 25.0f));

			EndPoint.Y = EndPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													   interval + FMath::RandRange(-25.0f, 25.0f));
		}
	}

	FVector Delta = EndPoint - StartPoint;

	float HorizontalDistance = FVector2D(Delta.X, Delta.Y).Size();
	float HeightDifference = Delta.Z;

	float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

	float FlightTime = 10.0f;

	// 목표 위치에 도달하기 위한 발사 각도 계산
	float Angle = FMath::Atan2(HeightDifference + 0.5f * Gravity * FlightTime * FlightTime,
							   HorizontalDistance);

	UBFLMissileAttack::MissileAttack(StartPoint,
									 EndPoint,
									 Angle,
									 FlightTime,
									 this,
									 GetWorld());
}

void ABossCharacter::MissileFire()
{
	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return;
	}

	ABOCharacter* Target = BossController->GetTarget();
	if (!Target)
	{
		return;
	}

	FVector StartPoint = FVector(-9700.0f, -5000.0f, 24000.0f);
	FVector EndPoint = Target->GetActorLocation();

	int32 interval = FMath::RandRange(1.0f, 16.0f) * 50;

	float RandX = FMath::RandRange(1.0f, 2.0f);
	float RandY = FMath::RandRange(1.0f, 2.0f);

	if (RandX == 1)
	{
		StartPoint.X = -(StartPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														 interval + FMath::RandRange(-25.0f, 25.0f)));

		EndPoint.X = -(EndPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													 interval + FMath::RandRange(-25.0f, 25.0f)));
	}
	else
	{
		StartPoint.X = StartPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													   interval + FMath::RandRange(-25.0f, 25.0f));

		EndPoint.X = EndPoint.X + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
												   interval + FMath::RandRange(-25.0f, 25.0f));
	}

	if (RandY == 1)
	{
		StartPoint.Y = -(StartPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
														 interval + FMath::RandRange(-25.0f, 25.0f)));

		EndPoint.Y = -(EndPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													 interval + FMath::RandRange(-25.0f, 25.0f)));
	}
	else
	{
		StartPoint.Y = StartPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
													   interval + FMath::RandRange(-25.0f, 25.0f));

		EndPoint.Y = EndPoint.Y + FMath::RandRange(-(interval + FMath::RandRange(-25.0f, 25.0f)),
												   interval + FMath::RandRange(-25.0f, 25.0f));
	}

	FVector Delta = EndPoint - StartPoint;

	float HorizontalDistance = FVector2D(Delta.X, Delta.Y).Size();
	float HeightDifference = Delta.Z;

	float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

	float FlightTime = 10.0f;

	// 목표 위치에 도달하기 위한 발사 각도 계산
	float Angle = FMath::Atan2(HeightDifference + 0.5f * Gravity * FlightTime * FlightTime,
							   HorizontalDistance);

	UBFLMissileAttack::MissileAttack(StartPoint,
									 EndPoint,
									 Angle,
									 FlightTime,
									 this,
									 GetWorld());
	FireCount = FireCount + 1;

	if (FireCount > 16)
	{
		FireCount = 0;
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(FireDelay,
										   this,
										   &ABossCharacter::MissileFire,
										   FMath::RandRange(0.05f, 0.07f),
										   false);
}

void ABossCharacter::ChangeMovement()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->GravityScale = 0.0f;
		Movement->SetMovementMode(MOVE_Flying);
		Movement->DefaultLandMovementMode = MOVE_Flying;
	}
}

void ABossCharacter::RecallSpawnPoint()
{
	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return;
	}
	FVector SpawnPoint = BossController->GetSpawnPoint();
	SpawnPoint.Z = SpawnPoint.Z + 200;
	SetActorLocation(SpawnPoint);
}

void ABossCharacter::MissilePattern()
{

	ABossAIController* BossController = Cast<ABossAIController>(GetController());
	if (!BossController)
	{
		return;
	}

	MissileFire(false);

	GetWorld()->GetTimerManager().SetTimer(FireDelay,
										   this,
										   &ABossCharacter::MissileFire,
										   FMath::RandRange(0.05f, 0.07f),
										   false);

	BossController->PatternHold(EBossPattern::Missile);
}
