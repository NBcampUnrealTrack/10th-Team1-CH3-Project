#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "BulletBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USoundCue;

UCLASS()
class BEYONDOVERRIDE_API ABulletBase : public AActor
{
	GENERATED_BODY()

  protected:
	// Projectile Movement 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	// 피격 판정 콜리전
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	// 피격 파티클
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> HitParticle;
	// 피격 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundCue> HitSound;

	// 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 BaseDamage;

  public:
	ABulletBase();

	// 초기 설정
	virtual void Initialize(
		APawn* InInstigator,
		const int32 InBaseDamage,
		const FVector& Velocity,
		const float GravityScale = 1.f,
		const float LifeSpan = 0.f);

  protected:
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

	// 총알 피격 이벤트
	virtual void HandleImpact(AActor* OtherActor, const FHitResult& Hit);

	// 데미지 적용
	void ApplyDamage(AActor* OtherActor, int32 Damage);

	// 피격 이펙트 재생
	void PlayImpactEffects(
		const FVector& Location,
		const FRotator& Rotation);
};
