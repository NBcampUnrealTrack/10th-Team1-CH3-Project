#include "Projectiles/Throwables/ThrowableProjectile.h"

#include "Components/SphereComponent.h"

AThrowableProjectile::AThrowableProjectile()
{
	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(GetRootComponent());
}

void AThrowableProjectile::Initalize(
	APawn* InInstigator,
	int32 InDamage,
	const float InRadius,
	const float InDelay,
	const FVector& Velocity,
	const float GravityScale)
{
	Super::Initialize(
		InInstigator,
		InDamage,
		Velocity,
		GravityScale);

	// Instigator와의 충돌 무시
	Collision->IgnoreActorWhenMoving(InInstigator, true);

	// 반경 & 딜레이 저장
	Radius = InRadius;
	Delay = InDelay;

	// 콜리전 반경 적용
	Collision->SetSphereRadius(InRadius);

	// 타이머 활성화
	GetWorldTimerManager()
		.SetTimer(
			ActivateTimerHandle,
			this,
			&AThrowableProjectile::Activate,
			InDelay,
			false);
}

void AThrowableProjectile::Activate()
{
}
