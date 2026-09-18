#include "Projectiles/Bullets/BulletProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABulletProjectile::ABulletProjectile()
{
	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionProfileName(TEXT("Bullet"));
	Collision->SetNotifyRigidBodyCollision(true);

	// 충돌 이벤트 바인딩
	Collision->OnComponentHit.AddDynamic(
		this,
		&ABulletProjectile::OnHit);

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

void ABulletProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌 지점 디버그
	// DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Red, false, 2.0f);

	// 잘못된 충돌 로그 (Instigator or Bullet)
	if (IsValid(OtherActor))
	{
		if (OtherActor == GetInstigator() || OtherActor->IsA(ABulletProjectile::StaticClass()))
		{
			UE_LOG(LogTemp, Warning,
				   TEXT("HIT | SelfActor=%s | HitComp=%s | OtherActor=%s | OtherComp=%s | OtherOwner=%s | Instigator=%s"),
				   *GetNameSafe(this),
				   *GetNameSafe(HitComponent),
				   *GetNameSafe(OtherActor),
				   *GetNameSafe(OtherComp),
				   *GetNameSafe(OtherComp ? OtherComp->GetOwner() : nullptr),
				   *GetNameSafe(GetInstigator()));
		}
	}

	/// 데미지 적용
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

	// TODO: 피격 사운드

	Destroy();
}
