#pragma once

#include "CoreMinimal.h"

#include "Projectiles/ProjectileBase.h"

#include "BulletProjectile.generated.h"

class USphereComponent;

UCLASS()
class ABulletProjectile : public AProjectileBase
{
	GENERATED_BODY()

  protected:
	TObjectPtr<USphereComponent> Collision;

  public:
	ABulletProjectile();

	virtual void Initialize(
		APawn* InInstigator,
		const int32 InDamage,
		const FVector& Velocity,
		const float GravityScale = 1.f) override;

  protected:
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
