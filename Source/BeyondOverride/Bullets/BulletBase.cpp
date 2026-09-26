#include "Bullets/BulletBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->SetCollisionProfileName(TEXT("Bullet"));
	Collision->SetNotifyRigidBodyCollision(true); // Hit 이벤트 활성화
	Collision->SetGenerateOverlapEvents(true);    // Overlap 이벤트 활성화

	Collision->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);                   // Hit 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnBeginOverlap); // Begin Overlap 이벤트 바인딩

	// Projectile Movement 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = Collision;

	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bAutoActivate = false;
}

void ABulletBase::Initialize(
	APawn* InInstigator,
	const int32 InBaseDamage,
	const FVector& Velocity,
	const float GravityScale,
	const float LifeSpan)
{
	SetInstigator(InInstigator);

	BaseDamage = InBaseDamage;

	ProjectileMovement->Velocity = Velocity;
	ProjectileMovement->ProjectileGravityScale = GravityScale;
	ProjectileMovement->Activate();

	SetLifeSpan(LifeSpan);
}

void ABulletBase::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	HandleImpact(OtherActor, Hit);
}

void ABulletBase::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	HandleImpact(OtherActor, SweepResult);
}

void ABulletBase::HandleImpact(AActor* OtherActor, const FHitResult& Hit)
{
}

void ABulletBase::ApplyDamage(AActor* OtherActor, int32 Damage)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	// 데미지 적용
	UGameplayStatics::ApplyDamage(
		OtherActor,
		Damage,
		GetInstigatorController(),
		this,
		UDamageType::StaticClass());
}

void ABulletBase::PlayImpactEffects(const FVector& Location, const FRotator& Rotation)
{
	// 피격 파티클 재생
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			Location,
			Rotation);
	}

	// 피격 사운드 재생
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HitSound,
			Location);
	}
}
