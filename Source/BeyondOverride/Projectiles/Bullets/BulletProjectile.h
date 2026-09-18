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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	// 피격 파티클
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> HitParticle;

  public:
	ABulletProjectile();

  protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
