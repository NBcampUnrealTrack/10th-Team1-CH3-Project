// 26/09/23 Copyright CH3 Team1 Jinho Song

#pragma once

// Core include
#include "CoreMinimal.h"

// Base include
#include "GameFramework/Actor.h"

// UHT include
#include "AttackMissileActor.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;

UCLASS()
class BEYONDOVERRIDE_API AAttackMissileActor : public AActor
{
	GENERATED_BODY()

	// Methtods
  public:
	AAttackMissileActor();

	virtual float TakeDamage(float DamageAmount,
							 FDamageEvent const& DamageEvent,
							 AController* EventInstigator,
							 AActor* DamageCauser) override;

	void MissileSetUp(FVector Point,
					  float Angle,
					  int32 Damage,
					  ACharacter* ThisOwner);

	void MissileEffect();

  protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSecond) override;

	void Launch();

	void ExplosionSequnce(FVector Position);

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComp,
							AActor* OtherActor,
							UPrimitiveComponent* OhtherComp,
							int32 otherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						FVector NormalImpulse, const FHitResult& Hit);

	// Properties
  public:
  protected:
	FVector AttackPoint = FVector::ZeroVector;

	float FireAngle = 0.0f;

	uint32 BrokenCount = 0;

	uint32 ThisDamage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|AttackOwner")
	ACharacter* AttackOwner;

	// Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	UCapsuleComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|Component")
	UProjectileMovementComponent* ProjectileMovement;

	// Fly Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effect")
	TObjectPtr<UParticleSystem> TailEffect;

	// Explosion Effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effect")
	TObjectPtr<UParticleSystem> ExplosionEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Effect")
	TObjectPtr<USoundBase> ExplosionSound;

	FTimerHandle EffectUpdate;
};
