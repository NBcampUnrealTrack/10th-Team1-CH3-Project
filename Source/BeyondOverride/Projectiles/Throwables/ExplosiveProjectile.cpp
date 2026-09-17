#include "Projectiles/Throwables/ExplosiveProjectile.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AExplosiveProjectile::AExplosiveProjectile()
{
	ExplosionParticle = nullptr;
	ExplosionSound = nullptr;
}

void AExplosiveProjectile::Activate()
{
	const FVector ExplosionLocation = GetActorLocation();

	// 파티클 생성
	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			ExplosionLocation);
	}

	// 사운드 재생
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			ExplosionLocation);
	}

	// TODO: Camera Shake?

	// 범위 내 액터에 데미지 적용
	TArray<AActor*> OverlapActors;
	Collision->GetOverlappingActors(OverlapActors);

	for (AActor* Actor : OverlapActors)
	{
		if (Actor)
		{
			// 거리에 따라 데미지 조정
			float Distance = FVector::Distance(ExplosionLocation, Actor->GetActorLocation()); // 폭발 중심과의 거리
			float DamageMultiplier = 1.f - (Distance / Radius) * 0.5;                         // 적용되는 데미지 배율 (0.5 ~ 1)

			UGameplayStatics::ApplyDamage(
				Actor,
				DamageMultiplier * Damage,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass());
		}
	}

	// 액터 제거
	Destroy();
}
