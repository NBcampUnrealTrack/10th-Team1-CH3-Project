#include "Bullets/ExplosiveBullet.h"

#include "Components/SphereComponent.h"

AExplosiveBullet::AExplosiveBullet()
{
	// ExplosiveCollision 생성
	ExplosiveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Explosive Collision"));
	ExplosiveCollision->SetupAttachment(RootComponent);

	// MinDamageMultiplier 기본값 설정
	MinDamageMultiplier = 0.5f;
}

void AExplosiveBullet::HandleImpact(AActor* OtherActor, const FHitResult& Hit)
{
	// Instigator 또는 총알인 경우 무시
	if (IsValid(OtherActor))
	{
		if (OtherActor == GetInstigator() || OtherActor->IsA(ABulletBase::StaticClass()))
		{
			return;
		}
	}

	// 데미지 적용
	if (ExplosiveCollision)
	{
		const FVector& ExplosionLocation = Hit.Location;                            // 폭발 중심
		const float& ExplosionRadius = ExplosiveCollision->GetScaledSphereRadius(); // 폭발 반경

		// 폭발 범위 내 액터 배열 순회
		TArray<AActor*> OverlappingActors;
		ExplosiveCollision->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			if (!IsValid(Actor))
			{
				continue;
			}

			// 거리 비례 데미지 조절
			const float Distance = FVector::Distance(ExplosionLocation, Actor->GetActorLocation());  // 폭발 중심과의 거리
			const float DamageMultiplier = 1.f - (Distance / ExplosionRadius) * MinDamageMultiplier; // 데미지 배율

			ApplyDamage(Actor, DamageMultiplier * BaseDamage);
		}
	}

	// 피격 이펙트 재생
	PlayImpactEffects(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

	// 제거
	Destroy();
}
