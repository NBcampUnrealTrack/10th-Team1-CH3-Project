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
	// 피격 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundCue> HitSound;

  public:
	ABulletProjectile();

  protected:
	virtual void BeginPlay() override;

	// Hit 이벤트
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);
	// Begin Overlap 이벤트
	UFUNCTION()
	virtual void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 총알 피격 처리
	virtual void HandleImpact(AActor* OtherActor, const FHitResult& Hit);
};
