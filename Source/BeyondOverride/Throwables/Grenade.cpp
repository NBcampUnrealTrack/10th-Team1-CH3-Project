#include "Throwables/Grenade.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AGrenade::AGrenade()
{
	// ExplosiveCollision 생성
	ExplosiveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Explosive Collision"));
	ExplosiveCollision->SetupAttachment(RootComponent);

	// 멤버 변수 기본값 설정
	MinDamageMultiplier = 1.f;
	BaseDamage = 0;
}

void AGrenade::Activate()
{
	const FVector& ExplosionLocation = GetActorLocation();                      // 폭발 중심
	const float& ExplosionRadius = ExplosiveCollision->GetScaledSphereRadius(); // 폭발 반경

	// 데미지 적용
	if (ExplosiveCollision)
	{
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

			// 데미지 적용
			UGameplayStatics::ApplyDamage(
				Actor,
				DamageMultiplier * BaseDamage,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass());
		}
	}

	// 활성화 이펙트 재생
	PlayActivationEffects(ExplosionLocation, GetActorRotation());

	// 제거
	Destroy();
}
