// 26/09/23 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/Actor/AttackMissileActor.h"

// Add include
#include "Bullets/BulletBase.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/MonsterCharacter/MonsterCharacter.h"
#include "Monster/System/BFLSoundEvent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/Character/BOCharacter.h"

AAttackMissileActor::AAttackMissileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere Collision Comp"));
	SetRootComponent(CollisionComp);

	// ���� ��ȯ
	CollisionComp->SetCollisionProfileName(TEXT("Missile"));

	// ������ �ν�
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAttackMissileActor::OnCollisionOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &AAttackMissileActor::OnCollisionHit);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void AAttackMissileActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(EffectUpdate,
										   this,
										   &AAttackMissileActor::MissileEffect,
										   0.05f,
										   true);

	Launch();
}

void AAttackMissileActor::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);

	const FVector Velocity = ProjectileMovement->Velocity;

	if (!Velocity.IsNearlyZero())
	{
		FRotator Rotation = Velocity.Rotation();

		// �̻��� ���� �⺻ ���⿡ ���� ����
		Rotation.Pitch += 90.0f;

		SetActorRotation(Rotation);
	}
}

void AAttackMissileActor::Launch()
{
	FVector Delta = AttackPoint - CollisionComp->GetComponentLocation();

	float HorizontalDistance = FVector2D(Delta.X, Delta.Y).Size();
	float HeightDifference = Delta.Z;

	float Gravity = FMath::Abs(GetWorld()->GetGravityZ());

	float CosAngle = FMath::Cos(FireAngle);

	if (FMath::IsNearlyZero(CosAngle))
	{
		return;
	}

	// ������ ���� �ð��� ���� ������ �ʿ��� �ӵ� ���
	float Speed = HorizontalDistance / (FlightTime * CosAngle);

	float Pitch = FMath::RadiansToDegrees(FireAngle);

	FVector HorizontalDirection =
		FVector(Delta.X, Delta.Y, 0.0f).GetSafeNormal();

	FVector LaunchDirection =
		HorizontalDirection * CosAngle +
		FVector::UpVector * FMath::Sin(FireAngle);

	ProjectileMovement->Velocity = LaunchDirection * Speed;
}

void AAttackMissileActor::ExplosionSequnce(FVector Position)
{

	UParticleSystemComponent* Particle = nullptr;

	FVector ExplosionPoint = Position;

	// ��� �� ����� �迭
	TArray<FOverlapResult> OverlapResults;

	// Query ������ ����ü
	FCollisionObjectQueryParams ObjectQueryParams;

	// �ν��� Actor ���� ECC_Pawn
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(250.0f);

	bool bHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,
													 ExplosionPoint,
													 FQuat::Identity,
													 ObjectQueryParams,
													 CollisionShape);

	if (!AttackOwner)
	{
		return;
	}

	AMonsterCharacter* MonsterOnwer = Cast<AMonsterCharacter>(AttackOwner);
	if (!MonsterOnwer)
	{
		return;
	}

	MonsterOnwer->OnMissileHit(OverlapResults);

	if (ExplosionEffect)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
															ExplosionEffect,
															ExplosionPoint,
															FRotator::ZeroRotator,
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

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),
											  ExplosionSound,
											  GetActorLocation());
	}

	Destroy();
}

void AAttackMissileActor::OnCollisionOverlap(UPrimitiveComponent* OverlappedComp,
											 AActor* OtherActor,
											 UPrimitiveComponent* OhtherComp,
											 int32 otherBodyIndex,
											 bool bFromSweep,
											 const FHitResult& SweepResult)
{
	if (ABulletBase* HitActor = Cast<ABulletBase>(OtherActor))
	{
		return;
	}
	if (AAttackMissileActor* HitActor = Cast<AAttackMissileActor>(OtherActor))
	{
		return;
	}
	if (OtherActor == Owner)
	{
		return;
	}
	ExplosionSequnce(SweepResult.ImpactPoint);
}

void AAttackMissileActor::OnCollisionHit(UPrimitiveComponent* HitComponent,
										 AActor* OtherActor,
										 UPrimitiveComponent* OtherComp,
										 FVector NormalImpulse,
										 const FHitResult& Hit)
{
	if (ABulletBase* HitActor = Cast<ABulletBase>(OtherActor))
	{
		return;
	}
	ExplosionSequnce(Hit.ImpactPoint);
}

float AAttackMissileActor::TakeDamage(float DamageAmount,
									  FDamageEvent const& DamageEvent,
									  AController* EventInstigator,
									  AActor* DamageCauser)
{

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (BrokenCount >= 2)
	{
		ExplosionSequnce(GetActorLocation());
		BrokenCount = 0;
	}

	BrokenCount = BrokenCount + 1;

	return ActualDamage;
}

void AAttackMissileActor::MissileSetUp(FVector Point,
									   float Angle,
									   float Time,
									   ACharacter* ThisOwner)
{
	FlightTime = Time;
	AttackPoint = Point;
	FireAngle = Angle;
	AttackOwner = ThisOwner;
}

void AAttackMissileActor::MissileEffect()
{
	const FVector ReverseVelocity = -ProjectileMovement->Velocity;

	FBoxSphereBounds MissileBounds = CollisionComp->CalcLocalBounds();
	FTransform MissileTransform = CollisionComp->GetComponentTransform();
	FRotator MissileRotation = MissileTransform.GetRotation().Rotator();
	FVector MissileExtent = MissileBounds.BoxExtent;
	FVector MissileOrigin = MissileBounds.Origin;

	FVector TailLocation = GetActorLocation();

	FVector MissileLocalDirection;
	FVector MissileLocalEdge;
	FRotator Rotation;

	float TX = BIG_NUMBER;
	float TY = BIG_NUMBER;
	float TZ = BIG_NUMBER;

	if (!ReverseVelocity.IsNearlyZero())
	{
		Rotation = ReverseVelocity.Rotation();
		TailLocation = GetActorLocation() + ReverseVelocity.GetSafeNormal() * MissileExtent;
		MissileLocalDirection = MissileRotation.UnrotateVector(ReverseVelocity).GetSafeNormal();
	}

	if (!FMath::IsNearlyZero(MissileLocalDirection.X))
	{
		float XBoundary = MissileLocalDirection.X > 0.0f
							  ? MissileExtent.X
							  : -MissileExtent.X;

		TX = XBoundary / MissileLocalDirection.X;
	}

	if (!FMath::IsNearlyZero(MissileLocalDirection.Y))
	{
		float YBoundary = MissileLocalDirection.Y > 0.0f
							  ? MissileExtent.Y
							  : -MissileExtent.Y;

		TY = YBoundary / MissileLocalDirection.Y;
	}

	if (!FMath::IsNearlyZero(MissileLocalDirection.Z))
	{
		float ZBoundary = MissileLocalDirection.Z > 0.0f
							  ? MissileExtent.Z
							  : -MissileExtent.Z;

		TZ = ZBoundary / MissileLocalDirection.Z;
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

	MissileLocalEdge = MissileLocalDirection * T;
	FVector TailLocalLocation =
		MissileOrigin + MissileLocalEdge;

	TailLocation = MissileTransform.TransformPosition(TailLocalLocation);

	UParticleSystemComponent* Particle = nullptr;
	if (TailEffect)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
															TailEffect,
															TailLocation,
															Rotation,
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

	UBFLSoundEvent::SoundPlay(GetActorLocation(), "MissileFly", 50.0f, 1.0f, 0.2f, false, GetWorld());
}
