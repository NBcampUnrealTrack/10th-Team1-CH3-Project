#include "Projectiles/Bullets/BulletProjectile.h"

#include "BulletProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABulletProjectile::ABulletProjectile()
{
	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetCollisionProfileName(TEXT("Bullet"));
	Collision->SetNotifyRigidBodyCollision(true); // Hit 이벤트 활성화
	Collision->SetGenerateOverlapEvents(true);    // Overlap 이벤트 활성화

	// 피격 이벤트 바인딩
	Collision->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnHit);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABulletProjectile::OnBeginOverlap);

	// ProjectileMovement 설정
	ProjectileMovement->UpdatedComponent = Collision;
}

void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Instigator와의 충돌 무시
	if (Collision)
	{
		Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void ABulletProjectile::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	HandleImpact(OtherActor, Hit);
}

void ABulletProjectile::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	HandleImpact(OtherActor, SweepResult);
}

void ABulletProjectile::HandleImpact(AActor* OtherActor, const FHitResult& Hit)
{
	// Instigator 또는 총알 무시
	if (IsValid(OtherActor))
	{
		if (OtherActor == GetInstigator() || OtherActor->IsA(ABulletProjectile::StaticClass()))
		{
			return;
		}
	}

	// 데미지 적용
	if (IsValid(OtherActor) && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass());
	}

	// 피격 파티클
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation());
	}

	// 피격 사운드
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HitSound,
			Hit.ImpactPoint);
	}

	Destroy();
}
