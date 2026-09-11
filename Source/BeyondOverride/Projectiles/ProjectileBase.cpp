#include "Projectiles/ProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// SceneRoot 생성
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);

	// ProjectileMovement 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = SceneRoot;
	ProjectileMovement->bSweepCollision = true;

	// 기본 수명 설정
	InitialLifeSpan = 100.f;
}

void AProjectileBase::Initialize(
	APawn* InInstigator,
	const int32 InDamage,
	const FVector& Velocity,
	const float GravityScale)
{
	SetInstigator(InInstigator);

	Damage = InDamage;

	ProjectileMovement->Velocity = Velocity;
	ProjectileMovement->ProjectileGravityScale = GravityScale;
}
