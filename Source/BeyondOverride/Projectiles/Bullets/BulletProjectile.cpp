#include "Projectiles/Bullets/BulletProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABulletProjectile::ABulletProjectile()
{
	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetNotifyRigidBodyCollision(true);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);

	// 충돌 이벤트 바인딩
	Collision->OnComponentHit.AddDynamic(
		this,
		&ABulletProjectile::OnHit);

	// ProjectileMovement 설정
	ProjectileMovement->UpdatedComponent = Collision;

	// 콜리전 채널 설정 (총알 간 충돌 방지)
	Collision->SetCollisionProfileName(TEXT("Bullet"));
}

void ABulletProjectile::Initialize(
	APawn* InInstigator,
	const int32 InDamage,
	const FVector& Velocity,
	const float GravityScale,
	const float LifeSpan)
{
	Super::Initialize(
		InInstigator,
		InDamage,
		Velocity,
		GravityScale,
		LifeSpan);

	// Instigator와의 충돌 무시
	Collision->IgnoreActorWhenMoving(InInstigator, true);
}

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌 지점 디버그
	DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

	if (OtherActor && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass());

		// TODO: 피격 이펙트 및 사운드
	}

	Destroy();
}
