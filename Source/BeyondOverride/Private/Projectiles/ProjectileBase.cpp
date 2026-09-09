#include "Projectiles/ProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// SceneRoot 생성
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));

	// ProjectileMovement 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = SceneRoot;
	ProjectileMovement->bSweepCollision = true;

	// 기본 수명 설정
	InitialLifeSpan = 100.f;
}
