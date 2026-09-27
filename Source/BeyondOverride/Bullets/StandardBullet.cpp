#include "Bullets/StandardBullet.h"

void AStandardBullet::HandleImpact(AActor* OtherActor, const FHitResult& Hit)
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
	ApplyDamage(OtherActor, BaseDamage);

	// 피격 이펙트 재생
	PlayImpactEffects(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

	// 제거
	Destroy();
}
