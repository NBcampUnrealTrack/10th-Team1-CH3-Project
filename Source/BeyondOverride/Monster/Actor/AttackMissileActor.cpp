// 26/09/23 Copyright CH3 Team1 Jinho Song

// Base include
#include "Monster/Actor/AttackMissileActor.h"

// Add include
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AAttackMissileActor::AAttackMissileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere Collision Comp"));
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetCollisionProfileName(TEXT("Missile"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AAttackMissileActor::OnCollisionHit);
	CollisionComp->SetupAttachment(RootComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetupAttachment(RootComp);
}

void AAttackMissileActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAttackMissileActor::TargetPoint(FVector Point)
{
}

void AAttackMissileActor::ExplosionSequnce()
{
	FVector ExplosionPoint = GetActorLocation();

	UParticleSystemComponent* Particle = nullptr;

	// 결과 값 저장용 배열
	TArray<FOverlapResult> OverlapResults;

	// Query 설정용 구조체
	FCollisionObjectQueryParams ObjectQueryParams;

	// 인식할 Actor 설정 ECC_Pawn
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(150.0f);

	bool bHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,
													 ExplosionPoint,
													 FQuat::Identity,
													 ObjectQueryParams,
													 CollisionShape);

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();
		if (!Actor)
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(Actor,
									  ThisDamage,
									  AttackOwner->GetController(),
									  AttackOwner,
									  UDamageType::StaticClass());
	}

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

void AAttackMissileActor::OnCollisionHit(UPrimitiveComponent* HitComponent,
										 AActor* OtherActor,
										 UPrimitiveComponent* OtherComp,
										 FVector NormalImpulse,
										 const FHitResult& Hit)
{
	ExplosionSequnce();
}

float AAttackMissileActor::TakeDamage(float DamageAmount,
									  FDamageEvent const& DamageEvent,
									  AController* EventInstigator,
									  AActor* DamageCauser)
{

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (BrokenCount >= 5)
	{
		ExplosionSequnce();
		BrokenCount = 0;
	}

	BrokenCount = BrokenCount + 1;

	return ActualDamage;
}

void AAttackMissileActor::SetDamage(int32 Damage)
{
	ThisDamage = Damage;
}
